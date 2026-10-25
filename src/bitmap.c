/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      New bitmap routines.
 *
 *      By Elias Pschernig and Trent Gamblin.
 *
 *      See readme.txt for copyright information.
 */

/* Title: Bitmap routines
 */


#include <stdio.h>
#include <string.h>

#if (defined(__x86_64__) || defined(__i386__)) && (defined(__GNUC__) || defined(__clang__))
#include <tmmintrin.h>
#define ALLEGRO_HAVE_SSSE3_BYTESWAP
#endif
#include "allegro5/allegro.h"
#include "allegro5/internal/aintern.h"
#include "allegro5/internal/aintern_bitmap.h"
#include "allegro5/internal/aintern_display.h"
#include "allegro5/internal/aintern_pixels.h"
#include "allegro5/internal/aintern_shader.h"
#include "allegro5/internal/aintern_system.h"

ALLEGRO_DEBUG_CHANNEL("bitmap")


/* Creates a memory bitmap.
 */
static ALLEGRO_BITMAP *create_memory_bitmap(ALLEGRO_DISPLAY *current_display,
   int w, int h, int format, int flags)
{
   ALLEGRO_BITMAP *bitmap;
   int pitch;

   if (_al_pixel_format_is_video_only(format)) {
      /* Can't have a video-only memory bitmap... */
      return NULL;
   }

   format = _al_get_real_pixel_format(current_display, format);

   bitmap = al_calloc(1, sizeof *bitmap);

   pitch = w * al_get_pixel_size(format);

   bitmap->vt = NULL;
   bitmap->_format = format;

   /* If this is really a video bitmap, we add it to the list of to
    * be converted bitmaps.
    */
   bitmap->_flags = flags | ALLEGRO_MEMORY_BITMAP;
   bitmap->_flags &= ~ALLEGRO_VIDEO_BITMAP;
   bitmap->w = w;
   bitmap->h = h;
   bitmap->pitch = pitch;
   bitmap->_display = NULL;
   bitmap->locked = false;
   bitmap->cl = bitmap->ct = 0;
   bitmap->cr_excl = w;
   bitmap->cb_excl = h;
   al_identity_transform(&bitmap->transform);
   al_identity_transform(&bitmap->inverse_transform);
   bitmap->inverse_transform_dirty = false;
   al_identity_transform(&bitmap->proj_transform);
   al_orthographic_transform(&bitmap->proj_transform, 0, 0, -1.0, w, h, 1.0);
   bitmap->parent = NULL;
   bitmap->xofs = bitmap->yofs = 0;
   bitmap->memory = al_malloc(pitch * h);
   bitmap->use_bitmap_blender = false;
   bitmap->blender.blend_color = al_map_rgba(0, 0, 0, 0);
   al_get_new_bitmap_wrap(&bitmap->_wrap_u, &bitmap->_wrap_v);

   _al_register_convert_bitmap(bitmap);
   return bitmap;
}



static void destroy_memory_bitmap(ALLEGRO_BITMAP *bmp)
{
   _al_unregister_convert_bitmap(bmp);

   if (bmp->memory)
      al_free(bmp->memory);
   al_free(bmp);
}



ALLEGRO_BITMAP *_al_create_bitmap_params(ALLEGRO_DISPLAY *current_display,
   int w, int h, int format, int flags, int depth, int samples)
{
   ALLEGRO_SYSTEM *system = al_get_system_driver();
   ALLEGRO_BITMAP *bitmap;
   ALLEGRO_BITMAP **back;
   bool result;
   /* Reject bitmaps with negative dimensions.
    * Also reject bitmaps where a calculation pixel_size*w*h would overflow
    * int.  Supporting such bitmaps would require a lot more work.
    * Overflow calc based on https://stackoverflow.com/a/1514309/231929
    */
   if (w < 0 || h < 0 || (h > 0 && (int64_t) w > (INT_MAX/4) / (int64_t) h)) {
      ALLEGRO_WARN("Rejecting %dx%d bitmap\n", w, h);
      return NULL;
   }

   if ((flags & ALLEGRO_MEMORY_BITMAP) ||
         !current_display ||
         !current_display->vt ||
         current_display->vt->create_bitmap == NULL ||
         _al_vector_size(&system->displays) < 1)
   {
      if (flags & ALLEGRO_VIDEO_BITMAP)
         return NULL;

      return create_memory_bitmap(current_display, w, h, format, flags);
   }

   /* Else it's a display bitmap */

   bitmap = current_display->vt->create_bitmap(current_display, w, h,
      format, flags);
   if (!bitmap) {
      ALLEGRO_ERROR("failed to create display bitmap\n");
      return NULL;
   }

   bitmap->_display = current_display;
   bitmap->w = w;
   bitmap->h = h;
   bitmap->locked = false;
   bitmap->cl = 0;
   bitmap->ct = 0;
   bitmap->cr_excl = w;
   bitmap->cb_excl = h;
   al_identity_transform(&bitmap->transform);
   al_identity_transform(&bitmap->inverse_transform);
   bitmap->inverse_transform_dirty = false;
   al_identity_transform(&bitmap->proj_transform);
   al_orthographic_transform(&bitmap->proj_transform, 0, 0, -1.0, w, h, 1.0);
   bitmap->parent = NULL;
   bitmap->xofs = 0;
   bitmap->yofs = 0;
   bitmap->_flags |= ALLEGRO_VIDEO_BITMAP;
   bitmap->dirty = !(bitmap->_flags & ALLEGRO_NO_PRESERVE_TEXTURE);
   bitmap->_depth = depth;
   bitmap->_samples = samples;
   bitmap->use_bitmap_blender = false;
   bitmap->blender.blend_color = al_map_rgba(0, 0, 0, 0);
   al_get_new_bitmap_wrap(&bitmap->_wrap_u, &bitmap->_wrap_v);

   /* The display driver should have set the bitmap->memory field if
    * appropriate; video bitmaps may leave it NULL.
    */

   ASSERT(bitmap->pitch >= w * al_get_pixel_size(bitmap->_format));
   result = bitmap->vt->upload_bitmap(bitmap);

   if (!result) {
      al_destroy_bitmap(bitmap);
      if (flags & ALLEGRO_VIDEO_BITMAP)
         return NULL;
      /* With ALLEGRO_CONVERT_BITMAP, just use a memory bitmap instead if
      * video failed.
      */
      return create_memory_bitmap(current_display, w, h, format, flags);
   }

   /* We keep a list of bitmaps depending on the current display so that we can
    * convert them to memory bimaps when the display is destroyed. */
   back = _al_vector_alloc_back(&current_display->bitmaps);
   *back = bitmap;

   return bitmap;
}


/* Function: al_create_bitmap
 */
ALLEGRO_BITMAP *al_create_bitmap(int w, int h)
{
   ALLEGRO_BITMAP *bitmap;

   bitmap = _al_create_bitmap_params(al_get_current_display(), w, h,
      al_get_new_bitmap_format(), al_get_new_bitmap_flags(),
      al_get_new_bitmap_depth(), al_get_new_bitmap_samples());
   if (bitmap) {
      bitmap->dtor_item = _al_register_destructor(_al_dtor_list, "bitmap", bitmap,
         (void (*)(void *))al_destroy_bitmap);
   }

   return bitmap;
}


/* Function: al_destroy_bitmap
 */
void al_destroy_bitmap(ALLEGRO_BITMAP *bitmap)
{
   if (!bitmap) {
      return;
   }

   /* As a convenience, implicitly untarget the bitmap on the calling thread
    * before it is destroyed, but maintain the current display.
    */
   if (bitmap == al_get_target_bitmap()) {
      ALLEGRO_DISPLAY *display = al_get_current_display();
      if (display)
         al_set_target_bitmap(al_get_backbuffer(display));
      else
         al_set_target_bitmap(NULL);
   }

   _al_set_bitmap_shader_field(bitmap, NULL);

   _al_unregister_destructor(_al_dtor_list, bitmap->dtor_item);

   if (!al_is_sub_bitmap(bitmap)) {
      ALLEGRO_DISPLAY* disp = _al_get_bitmap_display(bitmap);
      if (al_get_bitmap_flags(bitmap) & ALLEGRO_MEMORY_BITMAP) {
         destroy_memory_bitmap(bitmap);
         return;
      }

      /* Else it's a display bitmap */

      if (bitmap->locked)
         al_unlock_bitmap(bitmap);

      if (bitmap->vt)
         bitmap->vt->destroy_bitmap(bitmap);

      if (disp)
         _al_vector_find_and_delete(&disp->bitmaps, &bitmap);

      if (bitmap->memory)
         al_free(bitmap->memory);
   }

   al_free(bitmap);
}


/* Function: al_convert_mask_to_alpha
 */
void al_convert_mask_to_alpha(ALLEGRO_BITMAP *bitmap, ALLEGRO_COLOR mask_color)
{
   ALLEGRO_LOCKED_REGION *lr;
   int x, y;
   ALLEGRO_COLOR pixel;
   ALLEGRO_COLOR alpha_pixel;
   ALLEGRO_STATE state;

   if (!(lr = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, 0))) {
      ALLEGRO_ERROR("Couldn't lock bitmap.");
      return;
   }

   al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
   al_set_target_bitmap(bitmap);

   alpha_pixel = al_map_rgba(0, 0, 0, 0);

   for (y = 0; y < bitmap->h; y++) {
      for (x = 0; x < bitmap->w; x++) {
         pixel = al_get_pixel(bitmap, x, y);
         if (memcmp(&pixel, &mask_color, sizeof(ALLEGRO_COLOR)) == 0) {
            al_put_pixel(x, y, alpha_pixel);
         }
      }
   }

   al_unlock_bitmap(bitmap);

   al_restore_state(&state);
}



/* Function: al_get_bitmap_width
 */
int al_get_bitmap_width(ALLEGRO_BITMAP *bitmap)
{
   return bitmap->w;
}



/* Function: al_get_bitmap_height
 */
int al_get_bitmap_height(ALLEGRO_BITMAP *bitmap)
{
   return bitmap->h;
}



/* Function: al_get_bitmap_format
 */
int al_get_bitmap_format(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_format;
   else
      return bitmap->_format;
}


int _al_get_bitmap_memory_format(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_memory_format;
   else
      return bitmap->_memory_format;
}



/* Function: al_get_bitmap_flags
 */
int al_get_bitmap_flags(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_flags;
   else
      return bitmap->_flags;
}


ALLEGRO_DISPLAY *_al_get_bitmap_display(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_display;
   else
      return bitmap->_display;
}


/* Function: al_get_bitmap_depth
 */
int al_get_bitmap_depth(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_depth;
   else
      return bitmap->_depth;
}


/* Function: al_get_bitmap_samples
 */
int al_get_bitmap_samples(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->parent)
      return bitmap->parent->_samples;
   else
      return bitmap->_samples;
}

/* Function: al_get_bitmap_blend_color
 */
ALLEGRO_COLOR al_get_bitmap_blend_color(void)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();
   ALLEGRO_BLENDER *b;

   ASSERT(bitmap);

   if (!bitmap->use_bitmap_blender) {
      /* If no bitmap blender set, use TLS */
      return al_get_blend_color();
   }

   b = &bitmap->blender;
   return b->blend_color;
}

/* Function: al_get_bitmap_blender
 */
void al_get_bitmap_blender(int *op, int *src, int *dst)
{
   al_get_separate_bitmap_blender(op, src, dst, NULL, NULL, NULL);
}

/* Function: al_get_separate_bitmap_blender
 */
void al_get_separate_bitmap_blender(int *op, int *src, int *dst, int *alpha_op, int *alpha_src, int *alpha_dst)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();
   ALLEGRO_BLENDER *b;

   ASSERT(bitmap);

   if (!bitmap->use_bitmap_blender) {
      /* If no bitmap blender set, use TLS */
      al_get_separate_blender(op, src, dst, alpha_op, alpha_src, alpha_dst);
      return;
   }

   b = &bitmap->blender;

   if (op)
      *op = b->blend_op;

   if (src)
      *src = b->blend_source;

   if (dst)
      *dst = b->blend_dest;

   if (alpha_op)
      *alpha_op = b->blend_alpha_op;

   if (alpha_src)
      *alpha_src = b->blend_alpha_source;

   if (alpha_dst)
      *alpha_dst = b->blend_alpha_dest;
}

/* Function: al_set_bitmap_blend_color
 */
void al_set_bitmap_blend_color(ALLEGRO_COLOR col)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();
   ALLEGRO_BLENDER *b;

   ASSERT(bitmap);

   b = &bitmap->blender;
   b->blend_color = col;
}

/* Function: al_set_bitmap_blender
 */
void al_set_bitmap_blender(int op, int src, int dest)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();

   ASSERT(bitmap);

   al_set_separate_bitmap_blender(op, src, dest, op, src, dest);
}

/* Function: al_set_separate_bitmap_blender
 */
void al_set_separate_bitmap_blender(int op, int src, int dst, int alpha_op, int alpha_src, int alpha_dst)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();
   ALLEGRO_BLENDER *b;

   ASSERT(bitmap);

   bitmap->use_bitmap_blender = true;
   b = &bitmap->blender;
   b->blend_op = op;
   b->blend_source = src;
   b->blend_dest = dst;
   b->blend_alpha_op = alpha_op;
   b->blend_alpha_source = alpha_src;
   b->blend_alpha_dest = alpha_dst;
}

/* Function: al_reset_bitmap_blender
 */
void al_reset_bitmap_blender(void)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();

   ASSERT(bitmap);

   bitmap->use_bitmap_blender = false;
   bitmap->blender.blend_color = al_map_rgba(0, 0, 0, 0);
}

/* Function: al_set_clipping_rectangle
 */
void al_set_clipping_rectangle(int x, int y, int width, int height)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();

   ASSERT(bitmap);

   if (x < 0) {
      width += x;
      x = 0;
   }
   if (y < 0) {
      height += y;
      y = 0;
   }
   if (x + width > bitmap->w) {
      width = bitmap->w - x;
   }
   if (y + height > bitmap->h) {
      height = bitmap->h - y;
   }
   if (width < 0) {
      width = 0;
   }
   if (height < 0) {
      height = 0;
   }

   bitmap->cl = x;
   bitmap->ct = y;
   bitmap->cr_excl = x + width;
   bitmap->cb_excl = y + height;

   if (bitmap->vt && bitmap->vt->update_clipping_rectangle) {
      bitmap->vt->update_clipping_rectangle(bitmap);
   }
}



/* Function: al_reset_clipping_rectangle
 */
void al_reset_clipping_rectangle(void)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();

   if (bitmap) {
      int w = al_get_bitmap_width(bitmap);
      int h = al_get_bitmap_height(bitmap);
      al_set_clipping_rectangle(0, 0, w, h);
   }
}



/* Function: al_get_clipping_rectangle
 */
void al_get_clipping_rectangle(int *x, int *y, int *w, int *h)
{
   ALLEGRO_BITMAP *bitmap = al_get_target_bitmap();

   ASSERT(bitmap);

   if (x) *x = bitmap->cl;
   if (y) *y = bitmap->ct;
   if (w) *w = bitmap->cr_excl - bitmap->cl;
   if (h) *h = bitmap->cb_excl - bitmap->ct;
}



/* Function: al_create_sub_bitmap
 */
ALLEGRO_BITMAP *al_create_sub_bitmap(ALLEGRO_BITMAP *parent,
   int x, int y, int w, int h)
{
   ALLEGRO_BITMAP *bitmap;

   if (parent->parent) {
      x += parent->xofs;
      y += parent->yofs;
      parent = parent->parent;
   }

   bitmap = al_calloc(1, sizeof *bitmap);
   bitmap->vt = parent->vt;

   /* Sub-bitmap inherits these from the parent.
    * Leave these unchanged so they can be detected if improperly accessed
    * directly. */
   bitmap->_format = 0;
   bitmap->_flags = 0;
   bitmap->_display = (ALLEGRO_DISPLAY*)0x1;
   bitmap->_wrap_u = 0;
   bitmap->_wrap_v = 0;

   bitmap->w = w;
   bitmap->h = h;
   bitmap->locked = false;
   bitmap->cl = bitmap->ct = 0;
   bitmap->cr_excl = w;
   bitmap->cb_excl = h;
   al_identity_transform(&bitmap->transform);
   al_identity_transform(&bitmap->inverse_transform);
   bitmap->inverse_transform_dirty = false;
   al_identity_transform(&bitmap->proj_transform);
   al_orthographic_transform(&bitmap->proj_transform, 0, 0, -1.0, w, h, 1.0);
   bitmap->shader = NULL;
   bitmap->parent = parent;
   bitmap->xofs = x;
   bitmap->yofs = y;
   bitmap->memory = NULL;

   bitmap->dtor_item = _al_register_destructor(_al_dtor_list, "sub_bitmap", bitmap,
      (void (*)(void *))al_destroy_bitmap);

   return bitmap;
}


/* Function: al_reparent_bitmap
 */
void al_reparent_bitmap(ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP *parent,
   int x, int y, int w, int h)
{
   ASSERT(bitmap->parent);
   /* Re-parenting a non-sub-bitmap makes no sense, so in release mode
    * just ignore it. */
   if (!bitmap->parent)
      return;

   if (parent->parent) {
      x += parent->xofs;
      y += parent->yofs;
      parent = parent->parent;
   }

   bitmap->parent = parent;
   bitmap->xofs = x;
   bitmap->yofs = y;
   bitmap->w = w;
   bitmap->h = h;
}


/* Function: al_is_sub_bitmap
 */
bool al_is_sub_bitmap(ALLEGRO_BITMAP *bitmap)
{
   return (bitmap->parent != NULL);
}


/* Function: al_get_parent_bitmap
 */
ALLEGRO_BITMAP *al_get_parent_bitmap(ALLEGRO_BITMAP *bitmap)
{
   ASSERT(bitmap);
   return bitmap->parent;
}


/* Function: al_get_bitmap_x
 */
int al_get_bitmap_x(ALLEGRO_BITMAP *bitmap)
{
   ASSERT(bitmap);
   return bitmap->xofs;
}


/* Function: al_get_bitmap_y
 */
int al_get_bitmap_y(ALLEGRO_BITMAP *bitmap)
{
   ASSERT(bitmap);
   return bitmap->yofs;
}


static bool transfer_bitmap_data(ALLEGRO_BITMAP *src, ALLEGRO_BITMAP *dst)
{
   ALLEGRO_LOCKED_REGION *dst_region;
   ALLEGRO_LOCKED_REGION *src_region;
   int src_format = al_get_bitmap_format(src);
   int dst_format = al_get_bitmap_format(dst);
   bool src_compressed = _al_pixel_format_is_compressed(src_format);
   bool dst_compressed = _al_pixel_format_is_compressed(dst_format);
   int copy_w = src->w;
   int copy_h = src->h;

   if (src_compressed && dst_compressed && src_format == dst_format) {
      int block_width = al_get_pixel_block_width(src_format);
      int block_height = al_get_pixel_block_height(src_format);
      if (!(src_region = al_lock_bitmap_blocked(src, ALLEGRO_LOCK_READONLY)))
         return false;

      if (!(dst_region = al_lock_bitmap_blocked(dst, ALLEGRO_LOCK_WRITEONLY))) {
         al_unlock_bitmap(src);
         return false;
      }
      copy_w = _al_get_least_multiple(copy_w, block_width);
      copy_h = _al_get_least_multiple(copy_h, block_height);
      ALLEGRO_DEBUG("Taking fast clone path.\n");
   }
   else {
      int lock_format = ALLEGRO_PIXEL_FORMAT_ANY;
      /* Go through a non-compressed intermediate */
      if (src_compressed && !dst_compressed) {
         lock_format = dst_format;
      }
      else if (!src_compressed && dst_compressed) {
         lock_format = src_format;
      }

      if (!(src_region = al_lock_bitmap(src, lock_format, ALLEGRO_LOCK_READONLY)))
         return false;

      if (!(dst_region = al_lock_bitmap(dst, lock_format, ALLEGRO_LOCK_WRITEONLY))) {
         al_unlock_bitmap(src);
         return false;
      }
   }

   if (src_region->format == dst_region->format) {
      _al_copy_bitmap_data(
         src_region->data, src_region->pitch,
         dst_region->data, dst_region->pitch,
         0, 0, 0, 0, copy_w, copy_h,
         src_region->format);
   }
   else {
      _al_convert_bitmap_data(
         src_region->data, src_region->format, src_region->pitch,
         dst_region->data, dst_region->format, dst_region->pitch,
         0, 0, 0, 0, copy_w, copy_h);
   }

   al_unlock_bitmap(src);
   al_unlock_bitmap(dst);

   return true;
}


void _al_copy_bitmap_data(
   const void *src, int src_pitch, void *dst, int dst_pitch,
   int sx, int sy, int dx, int dy, int width, int height,
   int format)
{
   int block_width = al_get_pixel_block_width(format);
   int block_height = al_get_pixel_block_height(format);
   int block_size = al_get_pixel_block_size(format);
   const char *src_ptr = src;
   char *dst_ptr = dst;
   int y;

   ASSERT(src);
   ASSERT(dst);
   ASSERT(_al_pixel_format_is_real(format));
   ASSERT(width % block_width == 0);
   ASSERT(height % block_height == 0);
   ASSERT(sx % block_width == 0);
   ASSERT(sy % block_height == 0);
   ASSERT(dx % block_width == 0);
   ASSERT(dy % block_height == 0);

   sx /= block_width;
   sy /= block_height;
   dx /= block_width;
   dy /= block_height;
   width /= block_width;
   height /= block_height;

   if ((src_ptr == dst_ptr) && (src_pitch == dst_pitch)) {
      return;
   }

   src_ptr += sy * src_pitch + sx * block_size;
   dst_ptr += dy * dst_pitch + dx * block_size;

   for (y = 0; y < height; y++) {
      memcpy(dst_ptr, src_ptr, width * block_size);
      src_ptr += src_pitch;
      dst_ptr += dst_pitch;
   }
}

#define CONV_R 0
#define CONV_G 1
#define CONV_B 2
#define CONV_A 3
#define CONV_X 4
#define CONV_FILL 0x80

static int format_32bpp_layout(int format, unsigned char layout[4])
{
#ifdef ALLEGRO_BIG_ENDIAN
   static const unsigned char argb[4] = {3, 0, 1, 2};
   static const unsigned char xrgb[4] = {4, 0, 1, 2};
   static const unsigned char abgr[4] = {3, 2, 1, 0};
   static const unsigned char xbgr[4] = {4, 2, 1, 0};
   static const unsigned char rgba[4] = {0, 1, 2, 3};
   static const unsigned char rgbx[4] = {0, 1, 2, 4};
#else
   static const unsigned char argb[4] = {2, 1, 0, 3};
   static const unsigned char xrgb[4] = {2, 1, 0, 4};
   static const unsigned char abgr[4] = {0, 1, 2, 3};
   static const unsigned char xbgr[4] = {0, 1, 2, 4};
   static const unsigned char rgba[4] = {3, 2, 1, 0};
   static const unsigned char rgbx[4] = {4, 2, 1, 0};
#endif
   static const unsigned char abgr_le[4] = {0, 1, 2, 3};
   const unsigned char *src = NULL;

   switch (format) {
      case ALLEGRO_PIXEL_FORMAT_ARGB_8888: src = argb; break;
      case ALLEGRO_PIXEL_FORMAT_XRGB_8888: src = xrgb; break;
      case ALLEGRO_PIXEL_FORMAT_ABGR_8888: src = abgr; break;
      case ALLEGRO_PIXEL_FORMAT_XBGR_8888: src = xbgr; break;
      case ALLEGRO_PIXEL_FORMAT_RGBA_8888: src = rgba; break;
      case ALLEGRO_PIXEL_FORMAT_RGBX_8888: src = rgbx; break;
      case ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE: src = abgr_le; break;
   }

   if (!src)
      return 0;
   memcpy(layout, src, 4);
   return 1;
}

static int format_24bpp_layout(int format, unsigned char layout[3])
{
#ifdef ALLEGRO_BIG_ENDIAN
   static const unsigned char rgb[3] = {0, 1, 2};
   static const unsigned char bgr[3] = {2, 1, 0};
#else
   static const unsigned char rgb[3] = {2, 1, 0};
   static const unsigned char bgr[3] = {0, 1, 2};
#endif
   const unsigned char *src = NULL;

   switch (format) {
      case ALLEGRO_PIXEL_FORMAT_RGB_888: src = rgb; break;
      case ALLEGRO_PIXEL_FORMAT_BGR_888: src = bgr; break;
   }

   if (!src)
      return 0;
   memcpy(layout, src, 3);
   return 1;
}

static int find_channel(const unsigned char *layout, int count, int channel)
{
   int i;

   for (i = 0; i < count; i++) {
      if (layout[i] == channel)
         return i;
   }
   return -1;
}

static int build_32bpp_mask(const unsigned char src_layout[4],
   const unsigned char dst_layout[4], unsigned char mask[16],
   unsigned char fill[16], unsigned char pixmask[4])
{
   int i, p;

   for (i = 0; i < 4; i++) {
      int channel = dst_layout[i];
      int source = -1;

      if (channel == CONV_X) {
         pixmask[i] = CONV_FILL;
      }
      else if (channel == CONV_A) {
         source = find_channel(src_layout, 4, CONV_A);
         pixmask[i] = (source < 0) ? CONV_FILL : source;
      }
      else {
         source = find_channel(src_layout, 4, channel);
         if (source < 0)
            return 0;
         pixmask[i] = source;
      }
   }

   for (p = 0; p < 4; p++) {
      for (i = 0; i < 4; i++) {
         int source = pixmask[i];
         int index = p * 4 + i;
         mask[index] = (source == CONV_FILL) ? CONV_FILL : p * 4 + source;
         fill[index] = (source == CONV_FILL) ? 0xFF : 0;
      }
   }

   return 1;
}

static int build_24bpp_mask(const unsigned char src_layout[3],
   const unsigned char dst_layout[4], unsigned char mask[16],
   unsigned char fill[16], unsigned char pixmask[4])
{
   int i, p;

   for (i = 0; i < 4; i++) {
      int channel = dst_layout[i];
      int source = -1;

      if (channel == CONV_A || channel == CONV_X) {
         pixmask[i] = CONV_FILL;
      }
      else {
         source = find_channel(src_layout, 3, channel);
         if (source < 0)
            return 0;
         pixmask[i] = source;
      }
   }

   for (p = 0; p < 4; p++) {
      for (i = 0; i < 4; i++) {
         int source = pixmask[i];
         int index = p * 4 + i;
         mask[index] = (source == CONV_FILL) ? CONV_FILL : p * 3 + source;
         fill[index] = (source == CONV_FILL) ? 0xFF : 0;
      }
   }

   return 1;
}


static int build_32_to_24_mask(const unsigned char src_layout[4],
   const unsigned char dst_layout[3], unsigned char mask[16],
   unsigned char pixmask[3])
{
   int i, p;

   for (i = 0; i < 3; i++) {
      int source = find_channel(src_layout, 4, dst_layout[i]);
      if (source < 0)
         return 0;
      pixmask[i] = source;
   }

   for (p = 0; p < 4; p++) {
      for (i = 0; i < 3; i++)
         mask[p * 3 + i] = p * 4 + pixmask[i];
   }
   for (i = 12; i < 16; i++)
      mask[i] = CONV_FILL;

   return 1;
}

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
__attribute__((target("ssse3")))
#endif
static void convert_32bpp_row(const unsigned char *src, unsigned char *dst,
   int width, const unsigned char mask[16], const unsigned char fill[16],
   const unsigned char pixmask[4])
{
   int x = 0;

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
   const __m128i maskv = _mm_loadu_si128((const __m128i *)mask);
   const __m128i fillv = _mm_loadu_si128((const __m128i *)fill);

   for (; x + 4 <= width; x += 4) {
      __m128i pixels = _mm_loadu_si128((const __m128i *)(src + x * 4));
      pixels = _mm_shuffle_epi8(pixels, maskv);
      pixels = _mm_or_si128(pixels, fillv);
      _mm_storeu_si128((__m128i *)(dst + x * 4), pixels);
   }
#endif

   for (; x < width; x++) {
      unsigned char in[4], out[4];
      int i;

      memcpy(in, src + x * 4, 4);
      for (i = 0; i < 4; i++) {
         if (pixmask[i] == CONV_FILL)
            out[i] = 0xFF;
         else
            out[i] = in[pixmask[i]];
      }
      memcpy(dst + x * 4, out, 4);
   }

   (void)fill;
}

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
__attribute__((target("ssse3")))
#endif
static void convert_24bpp_row(const unsigned char *src, unsigned char *dst,
   int width, const unsigned char mask[16], const unsigned char fill[16],
   const unsigned char pixmask[4])
{
   int x = 0;

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
   const __m128i maskv = _mm_loadu_si128((const __m128i *)mask);
   const __m128i fillv = _mm_loadu_si128((const __m128i *)fill);

   for (; x + 6 <= width; x += 4) {
      __m128i pixels = _mm_loadu_si128((const __m128i *)(src + x * 3));
      pixels = _mm_shuffle_epi8(pixels, maskv);
      pixels = _mm_or_si128(pixels, fillv);
      _mm_storeu_si128((__m128i *)(dst + x * 4), pixels);
   }
#endif

   for (; x < width; x++) {
      const unsigned char *in = src + x * 3;
      unsigned char out[4];
      int i;

      for (i = 0; i < 4; i++) {
         if (pixmask[i] == CONV_FILL)
            out[i] = 0xFF;
         else
            out[i] = in[pixmask[i]];
      }
      memcpy(dst + x * 4, out, 4);
   }

   (void)fill;
}

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
__attribute__((target("ssse3")))
#endif
static void convert_32bpp_to_24bpp_row(const unsigned char *src,
   unsigned char *dst, int width, const unsigned char mask[16],
   const unsigned char pixmask[3])
{
   int x = 0;

#ifdef ALLEGRO_HAVE_SSSE3_BYTESWAP
   const __m128i maskv = _mm_loadu_si128((const __m128i *)mask);

   for (; x + 4 <= width; x += 4) {
      __m128i pixels = _mm_loadu_si128((const __m128i *)(src + x * 4));
      uint32_t tail;
      pixels = _mm_shuffle_epi8(pixels, maskv);
      _mm_storel_epi64((__m128i *)(dst + x * 3), pixels);
      tail = (uint32_t)_mm_cvtsi128_si32(_mm_srli_si128(pixels, 8));
      memcpy(dst + x * 3 + 8, &tail, 4);
   }
#endif

   for (; x < width; x++) {
      int i;
      for (i = 0; i < 3; i++)
         dst[x * 3 + i] = src[x * 4 + pixmask[i]];
   }
}

static void convert_32bpp_to_24bpp(
   const void *src, int src_pitch,
   void *dst, int dst_pitch,
   int sx, int sy, int dx, int dy, int width, int height,
   int src_format, int dst_format)
{
   unsigned char src_layout[4], dst_layout[3];
   unsigned char mask[16];
   unsigned char pixmask[3];
   int y;

   if (!format_32bpp_layout(src_format, src_layout) ||
       !format_24bpp_layout(dst_format, dst_layout) ||
       !build_32_to_24_mask(src_layout, dst_layout, mask, pixmask))
      return;

   for (y = 0; y < height; y++) {
      const unsigned char *src_row = (const unsigned char *)src
         + (sy + y) * src_pitch + sx * 4;
      unsigned char *dst_row = (unsigned char *)dst
         + (dy + y) * dst_pitch + dx * 3;
      convert_32bpp_to_24bpp_row(src_row, dst_row, width, mask, pixmask);
   }
}

static void convert_32bpp(
   const void *src, int src_pitch,
   void *dst, int dst_pitch,
   int sx, int sy, int dx, int dy, int width, int height,
   int src_format, int dst_format)
{
   unsigned char src_layout[4], dst_layout[4];
   unsigned char mask[16], fill[16];
   unsigned char pixmask[4];
   int y;

   if (!format_32bpp_layout(src_format, src_layout) ||
       !format_32bpp_layout(dst_format, dst_layout) ||
       !build_32bpp_mask(src_layout, dst_layout,
          mask, fill, pixmask)) {
      fprintf(stderr, "allegro: attempted conversion %s -> %s (%dx%d)\n",
         _al_pixel_format_name((ALLEGRO_PIXEL_FORMAT)src_format),
         _al_pixel_format_name((ALLEGRO_PIXEL_FORMAT)dst_format),
         width, height);
      return;
   }

   for (y = 0; y < height; y++) {
      const unsigned char *src_row = (const unsigned char *)src
         + (sy + y) * src_pitch + sx * 4;
      unsigned char *dst_row = (unsigned char *)dst
         + (dy + y) * dst_pitch + dx * 4;
      convert_32bpp_row(src_row, dst_row, width,
         mask, fill, pixmask);
   }
}

static void convert_24bpp_to_32bpp(
   const void *src, int src_pitch,
   void *dst, int dst_pitch,
   int sx, int sy, int dx, int dy, int width, int height,
   int src_format, int dst_format)
{
   unsigned char src_layout[3], mi_layout[4], dst_layout[4];
   unsigned char mask24[16], fill24[16];
   unsigned char pixmask24[4];
   unsigned char mask32[16], fill32[16];
   unsigned char pixmask32[4];
   int mi_format;
   unsigned char *tmp;
   int y;

   if (!format_24bpp_layout(src_format, src_layout) ||
       !format_32bpp_layout(dst_format, dst_layout))
      return;

   mi_format = (src_format == ALLEGRO_PIXEL_FORMAT_RGB_888) ?
      ALLEGRO_PIXEL_FORMAT_ARGB_8888 : ALLEGRO_PIXEL_FORMAT_ABGR_8888;

   if (!format_32bpp_layout(mi_format, mi_layout) ||
       !build_24bpp_mask(src_layout, mi_layout,
          mask24, fill24, pixmask24) ||
       !build_32bpp_mask(mi_layout, dst_layout,
          mask32, fill32, pixmask32)) {
      fprintf(stderr, "allegro: attempted conversion %s -> %s (%dx%d)\n",
         _al_pixel_format_name((ALLEGRO_PIXEL_FORMAT)src_format),
         _al_pixel_format_name((ALLEGRO_PIXEL_FORMAT)dst_format),
         width, height);
      return;
   }

   tmp = al_malloc((size_t)width * 4);
   if (!tmp)
      return;

   for (y = 0; y < height; y++) {
      const unsigned char *src_row = (const unsigned char *)src
         + (sy + y) * src_pitch + sx * 3;
      unsigned char *dst_row = (unsigned char *)dst
         + (dy + y) * dst_pitch + dx * 4;

      convert_24bpp_row(src_row, tmp, width,
         mask24, fill24, pixmask24);
      convert_32bpp_row(tmp, dst_row, width,
         mask32, fill32, pixmask32);
   }

   al_free(tmp);
}

void _al_convert_bitmap_data(
   const void *src, int src_format, int src_pitch,
   void *dst, int dst_format, int dst_pitch,
   int sx, int sy, int dx, int dy, int width, int height)
{
   if (width <= 0 || height <= 0)
      return;

   if (src_format == dst_format) {
      _al_copy_bitmap_data(src, src_pitch, dst, dst_pitch,
         sx, sy, dx, dy, width, height, src_format);
      return;
   }

   {
      unsigned char layout24[3];
      if (format_24bpp_layout(dst_format, layout24)) {
         unsigned char layout32[4];
         if (format_32bpp_layout(src_format, layout32)) {
            convert_32bpp_to_24bpp(src, src_pitch, dst, dst_pitch,
               sx, sy, dx, dy, width, height, src_format, dst_format);
            return;
         }
      }
   }

   if (src_format == ALLEGRO_PIXEL_FORMAT_RGB_888 ||
       src_format == ALLEGRO_PIXEL_FORMAT_BGR_888) {
      unsigned char layout[4];
      if (format_32bpp_layout(dst_format, layout)) {
         convert_24bpp_to_32bpp(src, src_pitch, dst, dst_pitch,
            sx, sy, dx, dy, width, height, src_format, dst_format);
         return;
      }
   }

   convert_32bpp(src, src_pitch, dst, dst_pitch,
      sx, sy, dx, dy, width, height, src_format, dst_format);
}

/* Function: al_clone_bitmap
 */
ALLEGRO_BITMAP *al_clone_bitmap(ALLEGRO_BITMAP *bitmap)
{
   ALLEGRO_BITMAP *clone;
   ASSERT(bitmap);

   clone = al_create_bitmap(bitmap->w, bitmap->h);
   if (!clone)
      return NULL;
   if (!transfer_bitmap_data(bitmap, clone)) {
      al_destroy_bitmap(clone);
      return NULL;
   }
   return clone;
}


/* Function: al_backup_dirty_bitmap
 */
void al_backup_dirty_bitmap(ALLEGRO_BITMAP *bitmap)
{
   if (bitmap->vt && bitmap->vt->backup_dirty_bitmap)
      bitmap->vt->backup_dirty_bitmap(bitmap);
}


void _al_get_bitmap_wrap(ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP_WRAP *wrap_u, ALLEGRO_BITMAP_WRAP *wrap_v)
{
   ASSERT(bitmap);
   ASSERT(wrap_u);
   ASSERT(wrap_v);

   if (bitmap->parent) {
      bitmap = bitmap->parent;
   }

   *wrap_u = bitmap->_wrap_u;
   *wrap_v = bitmap->_wrap_v;
}

/* vim: set ts=8 sts=3 sw=3 et: */
