#include "matrix.h"
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

static PIO pio;
static uint sm;
static uint offset;
static uint32_t matrix[25];

#define WS2812_PIN 7

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline void refresh_matrix() {
    for (uint i = 0; i < 25; ++i) {
        put_pixel(pio, sm, matrix[i]);
    }
}

void clean_matrix(){
    for (uint i = 0; i < 25; i++) {
        matrix[i] = 0;
        put_pixel(pio, sm, 0);
    }
}

void put_pixel_at_position(uint x, uint y,uint32_t color) {
    for (uint i = 0; i < 25; ++i) {
        uint pos = 0;
        if (y % 2 == 0) {pos = y * 5 + (4-x);}
        else{pos = y * 5 + x;}
        matrix[pos] = color;
        refresh_matrix();
    }
}

void init_matrix(){
    stdio_init_all();
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}

void end_matrix(){
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
