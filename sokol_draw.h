/* sokol_draw.h -- https://github.com/takeiteasy/suk

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

#ifndef SUK_DRAW
#define SUK_DRAW
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SOKOL_GFX_INCLUDED
#error "Please include sokol_gfx.h before sokol_draw.h"
#endif

#if defined(__cplusplus)
}
#endif
#endif // SUK_DRAW

#ifdef SUK_IMPL
#ifndef __has_include
#define __has_include(x) 1
#endif
#ifndef SUK_DEPS_PATH
#define SUK_DEPS_PATH "deps"
#endif

#ifndef SOKOL_GL_INCLUDED
#if __has_include("sokol_gl.h")
#include "sokol_gl.h"
#else
#include "sokol/util/sokol_gl.h"
#endif
#endif

#ifndef SOKOL_GP_INCLUDED
#if __has_include("sokol_gp.h")
#include "sokol_gp.h"
#else
#include SUK_DEPS_PATH "/sokol_gp.h"
#endif
#endif

#endif // SUK_IMPL
