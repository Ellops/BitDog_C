#include "matrix.h"

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