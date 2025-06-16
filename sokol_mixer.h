/* sokol_mixer.h -- https://github.com/takeiteasy/suk

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

#ifndef SUK_MIXER
#define SUK_MIXER
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef __has_include
#define __has_include(x) 1
#endif

#ifndef SOKOL_AUDIO_INCLUDED
#if __has_include("sokol_audio.h")
#include "sokol_audio.h"
#else
#error Please include sokol_audio.h before the sokol_mixer.h implementation
#endif
#endif

#if defined(__cplusplus)
}
#endif
#endif // SUK_MIXER

#ifdef SUK_IMPL
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __has_include
#define __has_include(x) 1
#endif
#ifndef SUK_DEPS_PATH
#define SUK_DEPS_PATH "deps"
#endif

#if __has_include("stb_vorbis.h")
#include "stb_vorbis.h"
#else
#include SUK_DEPS_PATH "/stb_vorbis.h"
#endif

#define DR_WAV_IMPLEMENTATION
#if __has_include("dr_wav.h")
#include "dr_wav.h"
#else
#include SUK_DEPS_PATH "/dr_wav.h"
#endif

#define DR_MP3_IMPLEMENTATION
#if __has_include("dr_mp3.h")
#include "dr_mp3.h"
#else
#include SUK_DEPS_PATH "/dr_mp3.h"
#endif

#define QOA_IMPLEMENTATION
#if __has_include("qoa.h")
#include "qoa.h"
#else
#include SUK_DEPS_PATH "/qoa.h"
#endif

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_WIN32_IO
#if __has_include("dr_flac.h")
#include "dr_flac.h"
#else
#include SUK_DEPS_PATH "/dr_flac.h"
#endif

#endif // SUK_IMPL
