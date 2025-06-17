/* sokol_image.h -- https://github.com/takeiteasy/sokol-kit

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

#ifndef SK_IMG
#define SK_IMG
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef __has_include
#define __has_include(x) 1
#endif

#ifndef SOKOL_GFX_INCLUDED
#if __has_include("sokol_gfx.h")
#include "sokol_gfx.h"
#else
#error Please include sokol_gfx.h before sokol_image.h
#endif
#endif

#include <stdint.h>

typedef struct image_buffer {
    unsigned int width, height;
    int32_t *buffer;
} sg_image_buffer;

sg_image_buffer sg_empty_image(unsigned int w, unsigned int h);
bool sg_image_load_path(const char *path, sg_image_buffer *dst);
bool sg_image_load_from_memory(const void *data, size_t length, sg_image_buffer *dst);
void sg_destroy_image_buffer(sg_image_buffer *img);
void sg_image_pset(sg_image_buffer *img, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void sg_image_pget(sg_image_buffer *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

sg_image sg_empty_texture(unsigned int width, unsigned int height);
sg_image sg_load_texture_path(const char *path, unsigned int *width, unsigned int *height);
sg_image sg_load_texture_from_memory(unsigned char *data, size_t data_size, unsigned int *width, unsigned int *height);
sg_image sg_load_texture_from_buffer(sg_image_buffer *img);
void sg_update_texture_from_buffer(sg_image texture, sg_image_buffer *img);

#if defined(__cplusplus)
}
#endif
#endif // SK_INPUT

#ifdef SOKOL_IMPL
#ifdef _WIN32
#include <io.h>
#include <dirent.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#ifndef __has_include
#define __has_include(x) 1
#endif
#ifndef SK_DEPS_PATH
#define SK_DEPS_PATH "deps"
#endif

#define STB_IMAGE_IMPLEMENTATION
#if __has_include("stb_image.h")
#include "stb_image.h"
#else
#include SK_DEPS_PATH "/stb_image.h"
#endif

#define QOI_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#if __has_include("qoi.h")
#include "qoi.h"
#else
#include SK_DEPS_PATH "/qoi.h"
#endif

#define RGBA(R, G, B, A) (((unsigned int)(R) << 24) | ((unsigned int)(B) << 16) | ((unsigned int)(G) << 8) | (A))

static int does_file_exist(const char *path) {
    return !access(path, F_OK);
}

sg_image_buffer sg_empty_image(unsigned int w, unsigned int h) {
    assert(w > 0 && h > 0);
    size_t size = w * h * sizeof(int);
    sg_image_buffer result = {
        .width = w,
        .height = h,
        .buffer = malloc(size)
    };
    memset(result.buffer, 0, size);
    return result;
}

bool sg_image_load_path(const char *path, sg_image_buffer *dst) {
    bool result = false;
    unsigned char *data = NULL;
    if (!does_file_exist(path))
        return false;
    size_t sz = -1;
    FILE *fh = fopen(path, "rb");
    if (!fh)
        return false;
    fseek(fh, 0, SEEK_END);
    if (!(sz = ftell(fh)))
        goto BAIL;
    fseek(fh, 0, SEEK_SET);
    if (!(data = malloc(sz * sizeof(unsigned char))))
        goto BAIL;
    if (fread(data, sz, 1, fh) != 1)
        goto BAIL;
    result = sg_image_load_from_memory(data, (int)sz, dst);
BAIL:
    if (fh)
        fclose(fh);
    if (data)
        free(data);
    return result;
}

static int check_if_qoi(unsigned char *data) {
    return RGBA(data[0], data[0], data[0], data[0]) ==  RGBA('q', 'o', 'i', 'f');
}

bool sg_image_load_from_memory(const void *data, size_t data_size, sg_image_buffer *dst) {
    if (!data || data_size <= 0)
        return false;
    int _w, _h, c;
    unsigned char *img_data = NULL;
    if (check_if_qoi((unsigned char*)data)) {
        qoi_desc desc;
        if (!(img_data = qoi_decode(data, data_size, &desc, 4)))
            return false;
        _w = desc.width;
        _h = desc.height;
    } else
        if (!(img_data = stbi_load_from_memory(data, data_size, &_w, &_h, &c, 4)))
            return false;
    if (_w <= 0 || _h <= 0 || c < 3) {
        free(img_data);
        return false;
    }

    dst->width = _w;
    dst->height = _h;
    if (!(dst->buffer = malloc(_w * _h * sizeof(int)))) {
        free(img_data);
        return false;
    }
    for (int x = 0; x < _w; x++)
        for (int y = 0; y < _h; y++) {
            unsigned char *p = img_data + (x + _w * y) * 4;
            dst->buffer[y * _w + x] = RGBA(p[0], p[1], p[2], p[3]);
        }
    free(img_data);
    return true;
}

void sg_destroy_image_buffer(sg_image_buffer *img) {
    if (img && img->buffer) {
        free(img->buffer);
        memset(img, 0, sizeof(sg_image_buffer));
    }
}

void sg_image_pset(sg_image_buffer *img, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (img->buffer && x >= 0 && y >= 0 && x <= img->width && y <= img->height)
        img->buffer[y * img->width + x] = RGBA(r, g, b, a);
}

void sg_image_pget(sg_image_buffer *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    int color = 0;
    if (img->buffer && x >= 0 && y >= 0 && x <= img->width && y <= img->height)
        color = img->buffer[y * img->width + x];
    if (r)
        *r = (color >> 24) & 0xFF;
    if (g)
        *g = (color >> 16) & 0xFF;
    if (g)
        *g = (color >> 8) & 0xFF;
    if (a)
        *a = color & 0xFF;
}

sg_image sg_empty_texture(unsigned int width, unsigned int height) {
    if (width <= 0 || height <= 0)
        return (sg_image){.id=SG_INVALID_ID};
    sg_image_desc desc = {
        .width = width,
        .height = height,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .usage = SG_USAGE_STREAM
    };
    return sg_make_image(&desc);
}

static sg_image image_to_sg(sg_image_buffer *img) {
    sg_image texture = sg_empty_texture(img->width, img->height);
    sg_update_texture_from_buffer(texture, img);
    return texture;
}

sg_image sg_load_texture_path(const char *path, unsigned int *width, unsigned int *height) {
    sg_image_buffer tmp;
    if (!sg_image_load_path(path, &tmp))
        return (sg_image){.id=SG_INVALID_ID};
    if (width)
        *width = tmp.width;
    if (height)
        *height = tmp.height;
    return image_to_sg(&tmp);
}

sg_image sg_load_texture_from_memory(unsigned char *data, size_t data_size, unsigned int *width, unsigned int *height) {
    sg_image_buffer tmp;
    if (!sg_image_load_from_memory(data, data_size, &tmp))
        return (sg_image){.id=SG_INVALID_ID};
    if (width)
        *width = tmp.width;
    if (height)
        *height = tmp.height;
    return image_to_sg(&tmp);
}

void sg_update_texture_from_buffer(sg_image texture, sg_image_buffer *img) {
    if (texture.id == SG_INVALID_ID)
        return;
    sg_update_image(texture, &(sg_image_data) {
        .subimage[0][0] = (sg_range) {
            .ptr = img->buffer,
            .size = img->width * img->height * sizeof(int)
        }
    });
}
#endif
