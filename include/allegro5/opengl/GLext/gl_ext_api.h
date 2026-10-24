/* <Core> */

#ifdef _ALLEGRO_GL_VERSION_1_2
AGL_API(void, BlendColor, (GLclampf, GLclampf, GLclampf, GLclampf))
AGL_API(void, BlendEquation, (GLenum))
#endif

#if defined _ALLEGRO_GL_ARB_imaging
#endif

#if defined _ALLEGRO_GL_VERSION_1_3
AGL_API(void, ActiveTexture, (GLenum))
AGL_API(void, CompressedTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *))
AGL_API(void, GetCompressedTexImage, (GLenum, GLint, GLvoid *))
#endif

#if defined _ALLEGRO_GL_VERSION_1_4
AGL_API(void, BlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum))
AGL_API(void, WindowPos2i, (GLint, GLint))
#endif


#if defined _ALLEGRO_GL_VERSION_1_5
AGL_API(void, BindBuffer, (GLenum, GLuint))
AGL_API(void, DeleteBuffers, (GLsizei, const GLuint *))
AGL_API(void, GenBuffers, (GLsizei, GLuint *))
AGL_API(void, BufferData, (GLenum, GLsizeiptr, const GLvoid *, GLenum))
AGL_API(void, BufferSubData, (GLenum, GLintptr, GLsizeiptr, const GLvoid *))
AGL_API(void, GetBufferSubData, (GLenum, GLintptr, GLsizeiptr, GLvoid *))
#endif


#if defined _ALLEGRO_GL_VERSION_2_0
AGL_API(void, BlendEquationSeparate, (GLenum, GLenum))
AGL_API(GLuint, CreateProgram, (void))
AGL_API(GLuint, CreateShader, (GLenum))
AGL_API(void,   DeleteProgram, (GLuint))
AGL_API(void,   DeleteShader, (GLuint))
AGL_API(void,   AttachShader, (GLuint, GLuint))
AGL_API(void,   DetachShader, (GLuint, GLuint))
AGL_API(void,   ShaderSource, (GLuint, GLsizei, const GLchar **, const GLint *))
AGL_API(void,   CompileShader, (GLuint))
AGL_API(void,   LinkProgram, (GLuint))
AGL_API(void,   UseProgram, (GLuint))
AGL_API(void, Uniform1f, (GLint, GLfloat))
AGL_API(void, Uniform1i, (GLint, GLint))
AGL_API(void, Uniform1fv, (GLint, GLsizei, const GLfloat *))
AGL_API(void, Uniform2fv, (GLint, GLsizei, const GLfloat *))
AGL_API(void, Uniform3fv, (GLint, GLsizei, const GLfloat *))
AGL_API(void, Uniform4fv, (GLint, GLsizei, const GLfloat *))
AGL_API(void, Uniform1iv, (GLint, GLsizei, const GLint *))
AGL_API(void, Uniform2iv, (GLint, GLsizei, const GLint *))
AGL_API(void, Uniform3iv, (GLint, GLsizei, const GLint *))
AGL_API(void, Uniform4iv, (GLint, GLsizei, const GLint *))
AGL_API(void, UniformMatrix4fv, (GLint, GLsizei, GLboolean, const GLfloat *))
AGL_API(void, GetShaderiv,  (GLuint, GLenum, GLint *))
AGL_API(void, GetProgramiv, (GLuint, GLenum, GLint *))
AGL_API(void, GetShaderInfoLog,  (GLuint, GLsizei, GLsizei *, GLchar *))
AGL_API(void, GetProgramInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *))
AGL_API(GLint, GetUniformLocation, (GLuint, const GLchar *))
AGL_API(void, VertexAttribPointer,(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *))
AGL_API(void, EnableVertexAttribArray, (GLuint))
AGL_API(void, DisableVertexAttribArray, (GLuint))

AGL_API(GLint, GetAttribLocation, (GLuint, const GLchar *))



#endif


#if defined _ALLEGRO_GL_VERSION_2_1
#endif

#if defined _ALLEGRO_GL_VERSION_3_0
/* OpenGL 3.0 also reuses entry points from these extensions: */
/* ARB_framebuffer_object */
/* ARB_map_buffer_range */
/* ARB_vertex_array_object */
AGL_API(const GLubyte *, GetStringi, (GLenum, GLuint))
#endif


#if defined _ALLEGRO_GL_VERSION_3_1
/* OpenGL 3.1 also reuses entry points from these extensions: */
/* ARB_copy_buffer */
/* ARB_uniform_buffer_object */
#endif

#if defined _ALLEGRO_GL_VERSION_3_2
/* OpenGL 3.2 also reuses entry points from these extensions: */
/* ARB_draw_elements_base_vertex */
/* ARB_provoking_vertex */
/* ARB_sync */
/* ARB_texture_multisample */
#endif

#if defined _ALLEGRO_GL_VERSION_3_3
/* OpenGL 3.3 also reuses entry points from these extensions: */
/* ARB_blend_func_extended */
/* ARB_sampler_objects */
/* ARB_explicit_attrib_location, but it has none */
/* ARB_occlusion_query2 (no entry points) */
/* ARB_shader_bit_encoding (no entry points) */
/* ARB_texture_rgb10_a2ui (no entry points) */
/* ARB_texture_swizzle (no entry points) */
/* ARB_timer_query */
/* ARB_vertex_type_2_10_10_10_rev */
#endif

#if defined _ALLEGRO_GL_VERSION_4_3
#endif /* GL_VERSION_4_3 */


/* </Core> */
/* <ARB> */

#ifdef _ALLEGRO_GL_ARB_multitexture
#endif

#if defined _ALLEGRO_GL_ARB_transpose_matrix
#endif

#if defined _ALLEGRO_GL_ARB_multisample
#endif

#if defined _ALLEGRO_GL_ARB_texture_compression
#endif

#if defined _ALLEGRO_GL_ARB_point_parameters
#endif

#if defined _ALLEGRO_GL_ARB_vertex_blend
#endif

#if defined _ALLEGRO_GL_ARB_matrix_palette
#endif

#if defined _ALLEGRO_GL_ARB_window_pos
#endif

#if defined _ALLEGRO_GL_ARB_vertex_program
#endif

#if defined _ALLEGRO_GL_ARB_vertex_buffer_object
#endif

#if defined _ALLEGRO_GL_ARB_occlusion_query
#endif

#if defined _ALLEGRO_GL_ARB_shader_objects
#endif

#ifdef _ALLEGRO_GL_ARB_vertex_shader
#ifndef GL_ARB_vertex_program
#endif
#ifndef GL_ARB_vertex_program
#endif
#endif

#if defined _ALLEGRO_GL_ARB_draw_buffers
#endif

#if defined _ALLEGRO_GL_ARB_color_buffer_float
#endif

#if defined _ALLEGRO_GL_ARB_draw_instanced
#endif

#if defined _ALLEGRO_GL_ARB_framebuffer_object
AGL_API(void, BindRenderbuffer, (GLenum, GLuint))
AGL_API(void, DeleteRenderbuffers, (GLsizei, const GLuint *))
AGL_API(void, GenRenderbuffers, (GLsizei, GLuint *))
AGL_API(void, RenderbufferStorage, (GLenum, GLenum, GLsizei, GLsizei))
AGL_API(void, BindFramebuffer, (GLenum, GLuint))
AGL_API(void, DeleteFramebuffers, (GLsizei, const GLuint *))
AGL_API(void, GenFramebuffers, (GLsizei, GLuint *))
AGL_API(GLenum, CheckFramebufferStatus, (GLenum))
AGL_API(void, FramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint))
AGL_API(void, FramebufferRenderbuffer, (GLenum, GLenum, GLenum, GLuint))
AGL_API(void, GenerateMipmap, (GLenum))
AGL_API(void, RenderbufferStorageMultisample, (GLenum, GLsizei, GLenum, GLsizei, GLsizei))
#endif

#if defined _ALLEGRO_GL_ARB_geometry_shader4
#endif

#if defined _ALLEGRO_GL_ARB_instanced_arrays
#endif

#if defined _ALLEGRO_GL_ARB_map_buffer_range
#endif

#if defined _ALLEGRO_GL_ARB_texture_buffer_object
#endif

#if defined _ALLEGRO_GL_ARB_vertex_array_object
AGL_API(void, BindVertexArray, (GLuint))
AGL_API(void, GenVertexArrays, (GLsizei, GLuint *))
#endif

#if defined _ALLEGRO_GL_ARB_uniform_buffer_object
#endif

#if defined _ALLEGRO_GL_ARB_copy_buffer
#endif


#if defined _ALLEGRO_GL_ARB_draw_elements_base_vertex
#endif

#if defined _ALLEGRO_GL_ARB_provoking_vertex
#endif

#if defined _ALLEGRO_GL_ARB_sync
#endif

#if defined _ALLEGRO_GL_ARB_texture_multisample
#endif

#if defined _ALLEGRO_GL_ARB_draw_buffers_blend
#endif

#if defined _ALLEGRO_GL_ARB_sample_shading
#endif

#if defined _ALLEGRO_GL_ARB_shading_language_include
#endif

#if defined _ALLEGRO_GL_ARB_blend_func_extended
#endif

#if defined _ALLEGRO_GL_ARB_sampler_objects
#endif

#if defined _ALLEGRO_GL_ARB_timer_query
#endif

#if defined _ALLEGRO_GL_ARB_vertex_type_2_10_10_10_rev
#endif

#if defined _ALLEGRO_GL_ARB_draw_indirect
#endif

#if defined _ALLEGRO_GL_ARB_gpu_shader_fp64
#endif

#if defined _ALLEGRO_GL_ARB_shader_subroutine
#endif

#if defined _ALLEGRO_GL_ARB_tessellation_shader
#endif

#if defined _ALLEGRO_GL_ARB_transform_feedback2
#endif

#if defined _ALLEGRO_GL_ARB_transform_feedback3
#endif


/* </ARB> */


#if defined _ALLEGRO_GL_EXT_blend_color
#endif

#if defined _ALLEGRO_GL_EXT_polygon_offset
#endif

#if defined _ALLEGRO_GL_EXT_texture3D
#endif

#if defined _ALLEGRO_GL_SGIS_texture_filter4
#endif

#if defined _ALLEGRO_GL_EXT_subtexture
#endif

#if defined _ALLEGRO_GL_EXT_copy_texture
#endif

#if defined _ALLEGRO_GL_EXT_histogram
#endif

#if defined _ALLEGRO_GL_EXT_convolution
#endif

#if defined _ALLEGRO_GL_SGI_color_table
#endif

#if defined _ALLEGRO_GL_SGIX_pixel_texture
#endif

#if defined _ALLEGRO_GL_SGIS_pixel_texture
#endif

#if defined _ALLEGRO_GL_SGIS_texture4D
#endif

#if defined _ALLEGRO_GL_EXT_texture_object
#endif

#if defined _ALLEGRO_GL_SGIS_detail_texture
#endif

#if defined _ALLEGRO_GL_SGIS_sharpen_texture
#endif

#if defined _ALLEGRO_GL_SGIS_multisample
#endif

#if defined _ALLEGRO_GL_EXT_vertex_array
#endif

#if defined _ALLEGRO_GL_EXT_blend_minmax
#endif

#if defined _ALLEGRO_GL_SGIX_sprite
#endif

#if defined _ALLEGRO_GL_EXT_point_parameters
#endif

#if defined _ALLEGRO_GL_SGIS_point_parameters
#endif

#if defined _ALLEGRO_GL_SGIX_instruments
#endif

#if defined _ALLEGRO_GL_SGIX_framezoom
#endif

#if defined _ALLEGRO_GL_SGIX_tag_sample_buffer
#endif

#if defined _ALLEGRO_GL_SGIX_polynomial_ffd
#endif

#if defined _ALLEGRO_GL_SGIX_reference_plane
#endif

#if defined _ALLEGRO_GL_SGIX_flush_raster
#endif

#if defined _ALLEGRO_GL_SGIS_fog_function
#endif

#if defined _ALLEGRO_GL_HP_image_transform
#endif

#if defined _ALLEGRO_GL_EXT_color_subtable
#ifndef GL_EXT_paletted_texture
#endif
#endif

#if defined _ALLEGRO_GL_PGI_misc_hints
#endif

#if defined _ALLEGRO_GL_EXT_paletted_texture
#endif

#if defined _ALLEGRO_GL_SGIX_list_priority
#endif

#if defined _ALLEGRO_GL_EXT_index_material
#endif

#if defined _ALLEGRO_GL_EXT_index_func
#endif

#if defined _ALLEGRO_GL_EXT_compiled_vertex_array
#endif

#if defined _ALLEGRO_GL_EXT_cull_vertex
#endif

#if defined _ALLEGRO_GL_SGIX_fragment_lighting
#endif

#if defined _ALLEGRO_GL_EXT_draw_range_elements
#endif

#if defined _ALLEGRO_GL_EXT_light_texture
#endif

#if defined _ALLEGRO_GL_SGIX_async
#endif

#if defined _ALLEGRO_GL_INTEL_parallel_arrays
#endif

#if defined _ALLEGRO_GL_EXT_pixel_transform
#endif

#if defined _ALLEGRO_GL_EXT_secondary_color
#endif

#if defined _ALLEGRO_GL_EXT_texture_perturb_normal
#endif

#if defined _ALLEGRO_GL_EXT_multi_draw_arrays
#endif

#if defined _ALLEGRO_GL_EXT_fog_coord
#endif

#if defined _ALLEGRO_GL_EXT_coordinate_frame
#endif

#if defined _ALLEGRO_GL_SUNX_constant_data
#endif

#if defined _ALLEGRO_GL_SUN_global_alpha
#endif

#if defined _ALLEGRO_GL_SUN_triangle_list
#endif

#if defined _ALLEGRO_GL_SUN_vertex
#endif

#if defined _ALLEGRO_GL_EXT_blend_func_separate
#endif

#if defined _ALLEGRO_GL_INGR_blend_func_separate
#endif

#if defined _ALLEGRO_GL_EXT_vertex_weighting
#endif

#if defined _ALLEGRO_GL_NV_vertex_array_range
#endif

#if defined _ALLEGRO_GL_NV_register_combiners
#endif

#if defined _ALLEGRO_GL_MESA_resize_buffers
#endif

#if defined _ALLEGRO_GL_MESA_window_pos
#endif

#if defined _ALLEGRO_GL_IBM_multimode_draw_arrays
#endif

#ifdef AGK_IBM_vertex_array_lists
#endif

#if defined _ALLEGRO_GL_3DFX_tbuffer
#endif

#if defined _ALLEGRO_GL_EXT_multisample
#endif

#if defined _ALLEGRO_GL_SGIS_texture_color_mask
#endif

#if defined _ALLEGRO_GL_SGIX_igloo_interface
#endif

#if defined _ALLEGRO_GL_NV_fence
#endif

#if defined _ALLEGRO_GL_NV_evaluators
#endif

#if defined _ALLEGRO_GL_NV_register_combiners2
#endif

#if defined _ALLEGRO_GL_NV_vertex_program
#endif

#if defined _ALLEGRO_GL_ATI_envmap_bumpmap
#endif

#if defined _ALLEGRO_GL_ATI_fragment_shader
#endif

#if defined _ALLEGRO_GL_ATI_pn_triangles
#endif

#if defined _ALLEGRO_GL_ATI_vertex_array_object
#endif

#if defined _ALLEGRO_GL_EXT_vertex_shader
#endif

#if defined _ALLEGRO_GL_ATI_vertex_streams
#endif

#if defined _ALLEGRO_GL_ATI_element_array
#endif

#if defined _ALLEGRO_GL_SUN_mesh_array
#endif

#if defined _ALLEGRO_GL_NV_occlusion_query
#endif

#if defined _ALLEGRO_GL_NV_point_sprite
#endif

#if defined _ALLEGRO_GL_EXT_stencil_two_side
#endif

#if defined _ALLEGRO_GL_APPLE_element_array
#endif

#if defined _ALLEGRO_GL_APPLE_fence
#endif

#if defined _ALLEGRO_GL_APPLE_vertex_array_object
#endif

#if defined _ALLEGRO_GL_APPLE_vertex_array_range
#endif

#if defined _ALLEGRO_GL_ATI_draw_buffers
#endif

#if defined _ALLEGRO_GL_NV_fragment_program
#endif

#if defined _ALLEGRO_GL_NV_half_float
#endif

#if defined _ALLEGRO_GL_NV_pixel_data_range
#endif

#if defined _ALLEGRO_GL_NV_primitive_restart
#endif

#if defined _ALLEGRO_GL_ATI_map_object_buffer
#endif

#if defined _ALLEGRO_GL_ATI_separate_stencil
#endif

#if defined _ALLEGRO_GL_ATI_vertex_attrib_array_object
#endif

#if defined _ALLEGRO_GL_OES_byte_coordinates
#endif

#if defined _ALLEGRO_GL_OES_fixed_point
#endif

#if defined _ALLEGRO_GL_OES_single_precision
#endif

#if defined _ALLEGRO_GL_OES_query_matrix
#endif

#if defined _ALLEGRO_GL_EXT_depth_bounds_test
#endif


#if defined _ALLEGRO_GL_EXT_blend_equation_separate
#endif


#if defined _ALLEGRO_GL_EXT_framebuffer_object
AGL_API(void, BindRenderbufferEXT, (GLenum, GLuint))
AGL_API(void, DeleteRenderbuffersEXT, (GLsizei, const GLuint *))
AGL_API(void, GenRenderbuffersEXT, (GLsizei, GLuint *))
AGL_API(void, RenderbufferStorageEXT, (GLenum, GLenum, GLsizei, GLsizei))
AGL_API(void, BindFramebufferEXT, (GLenum, GLuint))
AGL_API(void, DeleteFramebuffersEXT, (GLsizei, const GLuint *))
AGL_API(void, GenFramebuffersEXT, (GLsizei, GLuint *))
AGL_API(GLenum, CheckFramebufferStatusEXT, (GLenum))
AGL_API(void, FramebufferTexture2DEXT, (GLenum, GLenum, GLenum, GLuint, GLint))
AGL_API(void, FramebufferRenderbufferEXT, (GLenum, GLenum, GLenum, GLuint))
AGL_API(void, GenerateMipmapEXT, (GLenum))
#endif

#if defined _ALLEGRO_GL_GREMEDY_string_marker
#endif

#if defined _ALLEGRO_GL_EXT_stencil_clear_tag
#endif

#if defined _ALLEGRO_GL_EXT_framebuffer_blit
AGL_API(void, BlitFramebufferEXT, (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum))
#endif

#if defined _ALLEGRO_GL_EXT_framebuffer_multisample
AGL_API(void, RenderbufferStorageMultisampleEXT, (GLenum, GLsizei, GLenum, GLsizei, GLsizei))
#endif

#if defined _ALLEGRO_GL_EXT_timer_query
#endif

#if defined _ALLEGRO_GL_EXT_gpu_program_parameters
#endif

#if defined _ALLEGRO_GL_APPLE_flush_buffer_range
#endif

#if defined _ALLEGRO_GL_EXT_bindable_uniform
#endif

#if defined _ALLEGRO_GL_EXT_draw_buffers2
#endif

#if defined _ALLEGRO_GL_EXT_draw_instanced
#endif

#if defined _ALLEGRO_GL_EXT_geometry_shader4
#if !defined _ALLEGRO_GL_EXT_texture_array
#endif
#endif

#if defined _ALLEGRO_GL_EXT_gpu_shader4
#endif

#if defined _ALLEGRO_GL_EXT_texture_array
#endif

#if defined _ALLEGRO_GL_EXT_texture_buffer_object
#endif

#if defined _ALLEGRO_GL_texture_integer
#endif

#if defined _ALLEGRO_GL_NV_depth_buffer_float
#endif

#if defined _ALLEGRO_GL_NV_framebuffer_multisample_coverage
#endif

#if defined _ALLEGRO_GL_NV_geometry_program4
#if !defined _ALLEGRO_GL_EXT_geometry_shader4
#if !defined _ALLEGRO_GL_EXT_texture_array
#endif
#endif
#endif

#if defined _ALLEGRO_GL_NV_gpu_program4
#endif

#if defined _ALLEGRO_GL_NV_parameter_buffer_object
#if !defined _ALLEGRO_GL_NV_transform_feedback
#endif
#if !defined _ALLEGRO_GL_EXT_draw_buffers2
#endif
#endif

#if defined _ALLEGRO_GL_NV_transform_feedback
#if !defined _ALLEGRO_GL_EXT_draw_buffers2
/*AGL_API(void, GetIntegerIndexedvEXT, (GLenum, GLuint, GLint *))*/
#endif
#endif

#if defined _ALLEGRO_GL_NV_vertex_program4
#ifndef _ALLEGRO_GL_EXT_gpu_shader4
#endif
#endif

#if defined _ALLEGRO_GL_GREMEDY_frame_terminator
#endif

#if defined _ALLEGRO_GL_NV_conditional_render
#endif

#if defined _ALLEGRO_GL_EXT_transform_feedback
#endif

#if defined _ALLEGRO_GL_EXT_direct_state_access
#endif

#if defined _ALLEGRO_GL_NV_explicit_multisample
#endif

#if defined _ALLEGRO_GL_NV_transform_feedback2
#endif

#if defined _ALLEGRO_GL_AMD_performance_monitor
#endif

#if defined _ALLEGRO_GL_AMD_vertex_shader_tesselator
#endif

#if defined _ALLEGRO_GL_EXT_provoking_vertex
#endif

#if defined _ALLEGRO_GL_AMD_draw_buffers_blend
#endif

#if defined _ALLEGRO_GL_APPLE_texture_range
#endif

#if defined _ALLEGRO_GL_APPLE_vertex_program_evaluators
#endif

#if defined _ALLEGRO_GL_APPLE_object_purgeable
#endif

#if defined _ALLEGRO_GL_NV_video_capture
#endif

#if defined _ALLEGRO_GL_EXT_separate_shader_objects
#endif

#if defined _ALLEGRO_GL_NV_shader_buffer_load
#endif

#if defined _ALLEGRO_GL_NV_vertex_buffer_unified_memory
#endif

#if defined _ALLEGRO_GL_NV_texture_barrier
#endif
