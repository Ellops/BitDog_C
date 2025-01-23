/*
MIT License

Copyright (c) 2025 Ewerton Lopes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _inc_matrix
#define _inc_matrix

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "matrix.pio.h"

#define WS2812_PIN 7
#define IS_RGBW false

#define URGB_U32(r, g, b) \
    ((uint32_t)((r) << 8) | \
     (uint32_t)((g) << 16) | \
     (uint32_t)(b))

#define U32_RED URGB_U32(0x60, 0, 0)
#define U32_YELLOW URGB_U32(0x60, 0x60, 0)
#define U32_GREEN URGB_U32(0, 0x60, 0)


void clean_matrix();

void put_pixel_at_position(uint x, uint y,uint32_t color);

void init_matrix();

void end_matrix();

#endif