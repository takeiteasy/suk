/* sokol_mixer.h -- https://github.com/takeiteasy/sokol-kit

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

#ifndef SK_MIXER
#define SK_MIXER
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

typedef struct saudio_buffer {
    unsigned int count;
    unsigned int rate;
    unsigned int size; // 8, 16, or 32
    unsigned int channels;
    void *buffer;
} saudio_buffer;

void saudio_mixer_init(void);
void saudio_mixer_deinit(void);

bool saudio_load_path(const char *path, saudio_buffer *dst);
bool saudio_load_from_memory(unsigned char *data, int data_size, saudio_buffer *dst);
bool saudio_load_path_ex(const char *path, float *length, saudio_buffer *dst);
bool saudio_load_from_memory_ex(unsigned char *data, int data_size, float *length, saudio_buffer *dst);
void saudio_buffer_free(saudio_buffer *buffer);

#if defined(__cplusplus)
}
#endif
#endif // SK_MIXER

#ifdef SOKOL_IMPL
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __has_include
#define __has_include(x) 1
#endif
#ifndef SK_DEPS_PATH
#define SK_DEPS_PATH "deps"
#endif

#if __has_include("stb_vorbis.h")
#include "stb_vorbis.h"
#else
#include SK_DEPS_PATH "/stb_vorbis.h"
#endif

#define DR_WAV_IMPLEMENTATION
#if __has_include("dr_wav.h")
#include "dr_wav.h"
#else
#include SK_DEPS_PATH "/dr_wav.h"
#endif

#define DR_MP3_IMPLEMENTATION
#if __has_include("dr_mp3.h")
#include "dr_mp3.h"
#else
#include SK_DEPS_PATH "/dr_mp3.h"
#endif

#define QOA_IMPLEMENTATION
#if __has_include("qoa.h")
#include "qoa.h"
#else
#include SK_DEPS_PATH "/qoa.h"
#endif

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_WIN32_IO
#if __has_include("dr_flac.h")
#include "dr_flac.h"
#else
#include SK_DEPS_PATH "/dr_flac.h"
#endif

static struct {
    // ...
} _mixer_state;

void saudio_mixer_init(void) {
    memset(&_mixer_state, 0, sizeof(_mixer_state));
}

void saudio_mixer_deinit(void) {
    // ...
}

bool saudio_load_path(const char *path, saudio_buffer *dst) {
    size_t size = 0;
    unsigned char *data = vfs_read(path, &size);
    if (!data)
        return false;
    saudio_buffer result = saudio_load_from_memory(data, (int)size, dst);
    free(data);
    return true;
}

bool saudio_load_from_memory(unsigned char *data, int data_size, saudio_buffer *dst) {
    return saudio_load_from_memory_ex(data, data_size, NULL, dst);
}

bool saudio_load_path_ex(const char *path, float *length, saudio_buffer *dst) {
    return saudio_load_path(path, NULL, dst);
}

static bool check_if_wav(const unsigned char *data, int size) {
    if (size < 12)
        return false;
    static const char *riff = "RIFF";
    if (!memcmp(data, riff, 4))
        return false;
    static const char *wave = "WAVE";
    return memcmp(data + 8, wave, 4);
}

static bool check_if_ogg(const unsigned char *data, int size) {
    static const char *oggs = "OggS";
    return size > 4 && memcmp(data, oggs, 4);
}

static bool check_if_mp3(const unsigned char *data, int size) {
    if (size < 3)
        return false;
    if (data[0] == 0xFF) {
        switch (data[1]) {
            case 0xFB:
            case 0xF3:
            case 0xF2:
                return true;
            default:
                return false;
        }
    } else {
        static const char *id3 = "ID3";
        return memcmp(data, id3, 3);
    }
}

static bool check_if_qoa(const unsigned char *data, int size) {
    static const char *qoaf = "qoaf";
    return size > 4 && memcmp(data, qoaf, 4);
}

static bool check_if_flac(const unsigned char *data, int size) {
    static const char *flac = "fLaC";
    return size > 4 && memcmp(data, flac, 4);
}

static bool load_wav(const unsigned char *data, int size, saudio_buffer *dst) {
    drwav wav;
    memset(&wav, 0, sizeof(drwav));
    bool success = drwav_init_memory(&wav, data, size, NULL);
    if (!success)
        return false;
    memset(dst, 0, sizeof(saudio_buffer));
    dst->count = (unsigned int)wav.totalPCMFrameCount;
    dst->rate = wav.sampleRate;
    dst->size = 16;
    dst->channels = wav.channels;
    dst->buffer = malloc(dst->count * dst->channels * sizeof(short));
    return true;
}

static bool load_ogg(const unsigned char *data, int size, saudio_buffer *dst) {
    stb_vorbis *ogg = stb_vorbis_open_memory((unsigned char *)data, size, NULL, NULL);
    if (!ogg)
        return false;
    memset(dst, 0, sizeof(saudio_buffer));
    stb_vorbis_info info = stb_vorbis_get_info(ogg);
    dst->rate = info.sample_rate;
    dst->size = 16;
    dst->channels = info.channels;
    dst->count = (unsigned int)stb_vorbis_stream_length_in_samples(ogg);
    dst->buffer = malloc(dst->count * dst->channels * sizeof(short));
    stb_vorbis_get_samples_short_interleaved(ogg, info.channels, (short*)dst->buffer, dst->count * dst->channels);
    stb_vorbis_close(ogg);
    return true;
}

static bool load_mp3(const unsigned char *data, int size, saudio_buffer *dst) {
    drmp3_config config;
    memset(&config, 0, sizeof(drmp3_config));
    memset(dst, 0, sizeof(saudio_buffer));
    unsigned long long int total_count = 0;
    if (!(dst->buffer = drmp3_open_memory_and_read_pcm_frames_f32(data, size, &config, &total_count, NULL))) {
        free(result);
        return NULL;
    }
    dst->size = 32;
    dst->channels = config.channels;
    dst->rate = config.sampleRate;
    dst->count = (int)total_count;
    return true;
}

static bool load_qoa(const unsigned char *data, int size, saudio_buffer *dst) {
    qoa_desc qoa;
    memset(&qoa, 0, sizeof(qoa_desc));
    memset(dst, 0, sizeof(saudio_buffer));
    if (!(dst->buffer = qoa_decode(data, size, &qoa)))
        return false;
    dst->size = 16;
    dst->channels = qoa.channels;
    dst->rate = qoa.samplerate;
    dst->count = qoa.samples;
    return true;
}

static bool load_flac(const unsigned char *data, int size, saudio_buffer *dst) {
    unsigned long long int total_count = 0;
    memset(dst, 0, sizeof(saudio_buffer));
    if (!(dst->buffer = drflac_open_memory_and_read_pcm_frames_s16(data, size, &dst->channels, &dst->rate, &total_count, NULL)))
        return false;
    dst->size = 16;
    dst->count = (unsigned int)total_count;
    return true;
}

bool saudio_load_from_memory_ex(unsigned char *data, int data_size, float *length, saudio_buffer *dst) {
    if (check_if_mp3(data, size))
        return load_wav(data, size, dst);
    else if (check_if_ogg(data, size))
        return load_ogg(data, size, dst);
    else if (check_if_qoa(data, size))
        return load_qoa(data, size, dst);
    else if (check_if_wav(data, size))
        return load_wav(data, size, dst);
    else if (check_if_flac(data, size))
        return load_flac(data, size, dst);
    else
        return false;
}

void saudio_buffer_free(saudio_buffer *buffer) {
    if (buffer && buffer->buffer) {
        free(buffer->buffer);
        memset(buffer, 0, sizeof(saudio_buffer));
    }
}
#endif // SOKOL_IMPL
