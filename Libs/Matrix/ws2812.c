#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 7
#endif

// Check the pin is compatible with the platform
#if WS2812_PIN >= NUM_BANK0_GPIOS
#error Attempting to use a pin>=32 on a platform that does not support it
#endif

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

static inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            ((uint32_t) (w) << 24) |
            (uint32_t) (b);
}

void put_pixel_at_position(PIO pio, uint sm, uint x, uint y,uint32_t color) {
    for (uint i = 0; i < 25; ++i) {
        uint pos = 0;
        if (y % 2 == 0) {pos = y * 5 + (4-x);}
        else{pos = y * 5 + x;}
        if (pos == i)
            put_pixel(pio, sm, color);
        else
            put_pixel(pio, sm, 0);
    }
}

int main() {
    //set_sys_clock_48();
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    printf("WS2812 Smoke Test, using pin %d\n", WS2812_PIN);

    // todo get free sm
    PIO pio;
    uint sm;
    uint offset;

    // This will find a free pio and state machine for our program and load it for us
    // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
    // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int t = 0;;
    
    uint x,y;
    x = 0;
    y = 0;
    while (1) {

        adc_select_input(0);
        uint adc_y_raw = adc_read();
        adc_select_input(1);
        uint adc_x_raw = adc_read();

        const uint bar_width = 5;
        const uint adc_max = (1 << 12) - 1;
        uint bar_x_pos = adc_x_raw * bar_width / adc_max;
        uint bar_y_pos = adc_y_raw * bar_width / adc_max;
        printf("[%u,%u]\n",bar_x_pos,bar_y_pos);

        put_pixel_at_position(pio,sm,bar_x_pos,bar_y_pos,urgb_u32(0x20, 0, 0));

        sleep_ms(20);
    }

    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}