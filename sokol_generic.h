/* sokol_generic.h -- https://github.com/takeiteasy/suk

 _Generic wrapper around sokol types, call sg_query_info(<obj>) instead of
 sg_query_buffer_info or sg_query_image_info, for example.

 The MIT License (MIT)

 Copyright (c) 2024 George Watson

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#ifndef SUK_GENERIC
#define SUK_GENERIC
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SOKOL_GFX_INCLUDED
#error "Please include sokol_gfx.h before sokol_generic.h"
#endif

// NOTE: This library relies on the _Generic macro (introduced in C11)

#ifdef __has_extension
#if !__has_extension(c_generic_selections)
#error "This library relies on _Generic"
#endif
#else
#if !defined(__STDC__) || !__STDC__ || __STDC_VERSION__ < 201112L
#error "This library relies on _Generic"
#endif
#endif

#define sg_make(OBJ)                                                           \
  _Generic((OBJ),                                                              \
      const sg_buffer_desc *: sg_make_buffer,                                  \
      const sg_image_desc *: sg_make_image,                                    \
      const sg_sampler_desc *: sg_make_sampler,                                \
      const sg_shader_desc *: sg_make_shader,                                  \
      const sg_pipeline_desc *: sg_make_pipeline,                              \
      const sg_attachments_desc *: sg_make_attachments)(OBJ)

#define sg_destroy(OBJ)                                                        \
  _Generic((OBJ),                                                              \
      sg_buffer: sg_destroy_buffer,                                            \
      sg_image: sg_destroy_image,                                              \
      sg_sampler: sg_destroy_sampler,                                          \
      sg_shader: sg_destroy_shader,                                            \
      sg_pipeline: sg_destroy_pipeline,                                        \
      sg_attachments: sg_destroy_attachments)(OBJ)

#define sg_destroy_if_valid(OBJ)                                               \
  do {                                                                         \
    if (sg_query_state((OBJ)))                                                 \
      sg_destroy((OBJ));                                                       \
  } while (0)

#define sg_query_state(OBJ)                                                    \
  _Generic((OBJ),                                                              \
      sg_buffer: sg_query_buffer_state,                                        \
      sg_image: sg_query_image_state,                                          \
      sg_sampler: sg_query_sampler_state,                                      \
      sg_shader: sg_query_shader_state,                                        \
      sg_pipeline: sg_query_pipeline_state,                                    \
      sg_attachments: sg_query_attachments_state)(OBJ)

#define sg_query_info(OBJ)                                                     \
  _Generic((OBJ),                                                              \
      sg_buffer: sg_query_buffer_info,                                         \
      sg_image: sg_query_image_info,                                           \
      sg_sampler: sg_query_sampler_info,                                       \
      sg_shader: sg_query_shader_info,                                         \
      sg_pipeline: sg_query_pipeline_info,                                     \
      sg_attachments: sg_query_attachments_info)(OBJ)

#define sg_query_setup_desc(OBJ)                                               \
  _Generic((OBJ),                                                              \
      sg_buffer: sg_query_buffer_desc,                                         \
      sg_image: sg_query_image_desc,                                           \
      sg_sampler: sg_query_sampler_desc,                                       \
      sg_shader: sg_query_shader_desc,                                         \
      sg_pipeline: sg_query_pipeline_desc,                                     \
      sg_attachments: sg_query_attachments_desc)(OBJ)

#define sg_query_defaults(OBJ)                                                 \
  _Generic((OBJ),                                                              \
      const sg_buffer *: sg_query_buffer_defaults,                             \
      const sg_image *: sg_query_image_defaults,                               \
      const sg_sampler *: sg_query_sampler_defaults,                           \
      const sg_shader *: sg_query_shader_defaults,                             \
      const sg_pipeline *: sg_query_pipeline_defaults,                         \
      const sg_attachments *: sg_query_attachments_defaults)(OBJ)

#if defined(__cplusplus)
}
#endif
#endif // SUK_GENERIC
