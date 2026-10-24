#ifdef _ALLEGRO_GLX_VERSION_1_3
/* GLX 1.3 */
AGL_API(GLXFBConfig *, GetFBConfigs, (Display *, int, int *))
AGL_API(int, GetFBConfigAttrib, (Display *, GLXFBConfig, int, int *))
AGL_API(XVisualInfo *, GetVisualFromFBConfig, (Display *, GLXFBConfig))
AGL_API(GLXWindow, CreateWindow, (Display *, GLXFBConfig, Window, const int *))
AGL_API(void, DestroyWindow, (Display *, GLXWindow))
AGL_API(void, QueryDrawable, (Display *, GLXDrawable, int, unsigned int *))
AGL_API(GLXContext, CreateNewContext, (Display *, GLXFBConfig, int, GLXContext, Bool))
AGL_API(Bool, MakeContextCurrent, (Display *, GLXDrawable, GLXDrawable, GLXContext))
#endif
#ifdef _ALLEGRO_GLX_VERSION_1_4
/* GLX 1.4 */
AGL_API(__GLXextFuncPtr, GetProcAddress, (const GLubyte *))
#endif

#ifdef _ALLEGRO_GLX_ARB_get_proc_address
/* GLX_ARB_get_proc_address */
AGL_API(__GLXextFuncPtr, GetProcAddressARB, (const GLubyte *))
#endif

#ifdef _ALLEGRO_GLX_ARB_create_context
AGL_API(GLXContext, CreateContextAttribsARB, (Display *, GLXFBConfig, GLXContext, Bool, const int *))
#endif

#ifdef _ALLEGRO_GLX_SGI_swap_control
/* GLX_SGI_swap_control */
AGL_API(int, SwapIntervalSGI, (int))
#endif

#ifdef _ALLEGRO_GLX_SGI_video_sync
/* GLX_SGI_video_sync */
AGL_API(int, GetVideoSyncSGI, (unsigned int *))
AGL_API(int, WaitVideoSyncSGI, (int, int, unsigned int *))
#endif

#ifdef _ALLEGRO_GLX_SGI_make_current_read
/* GLX_SGI_make_current_read */
#endif

#ifdef _ALLEGRO_GLX_SGIX_video_source
/* GLX_SGIX_video_source */
/* This one needs SGI's header file <dmedia/vl.h> to be included first */
#ifdef _VL_H_
#endif
#endif

#ifdef _ALLEGRO_GLX_EXT_import_context
/* GLX_EXT_import_context */
#endif

#ifdef _ALLEGRO_GLX_SGIX_fbconfig
/* GLX_SGIX_fbconfig */
#endif

#ifdef _ALLEGRO_GLX_SGIX_pbuffer
/* GLX_SGIX_pbuffer */
#endif

#ifdef _ALLEGRO_GLX_SGI_cushion
/* GLX_SGI_cushion */
#endif

#ifdef _ALLEGRO_GLX_SGIX_video_resize
/* GLX_SGIX_video_resize */
#endif

#ifdef _ALLEGRO_GLX_SGIX_dmbuffer
/* GLX_SGIX_dmbuffer */
/* This one needs SGI's header file <dmedia/dm_buffer.h> to be included first */
#ifdef _DM_BUFFER_H_
#endif
#endif

#ifdef _ALLEGRO_GLX_SGIX_swap_group
/* GLX_SGIX_swap_group */
#endif

#ifdef _ALLEGRO_GLX_SGIX_swap_barrier
/* GLX_SGIX_swap_barrier */
#endif

#ifdef _ALLEGRO_GLX_SUN_get_transparent_index
/* GLX_SUN_get_transparent_index */
#endif

#ifdef _ALLEGRO_GLX_MESA_copy_sub_buffer
/* GLX_MESA_copy_sub_buffer */
#endif

#ifdef _ALLEGRO_GLX_MESA_pixmap_colormap
/* GLX_MESA_pixmap_colormap */
#endif

#ifdef _ALLEGRO_GLX_MESA_release_buffers
/* GLX_MESA_release_buffers */
#endif

#ifdef _ALLEGRO_GLX_MESA_set_3dfx_mode
/* GLX_MESA_set_3dfx_mode */
#endif

#ifdef _ALLEGRO_GLX_OML_sync_control
/* GLX_OML_sync_control */
#endif


#ifdef _ALLEGRO_GLX_SGIX_hyperpipe
#endif


#ifdef _ALLEGRO_GLX_MESA_agp_offset
#endif


#ifdef _ALLEGRO_GLX_EXT_texture_from_pixmap
#endif

#ifdef _ALLEGRO_GLX_NV_video_output
#endif

#ifdef _ALLEGRO_GLX_NV_swap_group
#endif

#ifdef _ALLEGRO_GLX_NV_video_capture
#endif

#ifdef _ALLEGRO_GLX_EXT_swap_control
#endif

#ifdef _ALLEGRO_GLX_NV_copy_image
#endif

#ifdef _ALLEGRO_GLX_EXT_create_context_es_profile
// no functions
#endif
