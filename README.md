# suk

> [!WARNING]
> Work in progres

```suk``` (sokol utility kit) is a small collection of utility libraries for [sokol](https://github.com/floooh/sokol/)

All libraries are single-header, so `SUK_IMPL` must be defined (***once***) before including any headers.

## Libraries

| File                  | Description                                   |
|-----------------------|-----------------------------------------------|
| **sokol_generic.h**   | _Generic wrappers for sokol types             |
| **sokol_image.h**†    | Load textures from memory or disk             |
| **sokol_input.h**     | Basic input manager (keyboard, mouse, etc)    |
| **sokol_mixer.h**†    | Audio loader + basic mixer (***WIP***)        |

* **†** Relies on third-party library (located in `deps/`)

## Libraries used

- test/main.c
    - [sokol_gp.h](https://github.com/edubart/sokol_gp/) [MIT]
- sokol_image.h
    - [stb_image.h](https://github.com/nothings/stb) [MIT or Public Domain]
    - [qoi.h](https://github.com/phoboslab/qoi/) [MIT]
- sokol_mixer.h
    - [dr_flac.h](https://github.com/mackron/dr_libs/) [MIT or Public Domain]
    - [dr_mp3.h](https://github.com/mackron/dr_libs/) [MIT or Public Domain]
    - [dr_wav.h](https://github.com/mackron/dr_libs/) [MIT or Public Domain]
    - [stb_vorbis.h](https://github.com/nothings/stb) [MIT or Public Domain]
    - [qoa.h](https://github.com/phoboslab/qoa/) [MIT]

## LICENSE
```
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
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
