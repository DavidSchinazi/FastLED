#pragma once

#include <stdint.h>
#include <assert.h>

inline uint8_t brightness_bitshifter8(uint8_t *brightness_src, uint8_t *brightness_dst, uint8_t max_shifts) {
    uint8_t src = *brightness_src;
    if (brightness_src == 0 || brightness_src == 0) {
        return 0;
    }
    // assert that there is a leading bit and no other bits set.
    assert( !(src & (src - 1) ));

    // Steal brightness from brightness_src and give it to brightness_dst.
    // After this function concludes the multiplication of brightness_dst and brightness_src will remain
    // constant.
    // This algorithm is a little difficult to follow and I don't understand why it works that well,
    // however I did work it out manually and has something to do with how numbers respond to bitshifts.
    uint8_t curr = *brightness_dst;
    uint8_t shifts = 0;
    for (uint8_t i = 0; i < max_shifts && src > 1; i++) {
        if (curr & 0b10000000) {
            // next shift will overflow
            break;
        }
        curr <<= 1;
        src >>= 1;
        shifts++;
    }
    // write out the output values.
    *brightness_dst = curr;
    *brightness_src = src;
    return shifts;
}

inline uint8_t brightness_bitshifter16(uint8_t *brightness_src, uint16_t *brightness_dst, uint8_t max_shifts, uint8_t steps=2) {
    uint8_t src = *brightness_src;
    if (brightness_src == 0 || brightness_src == 0) {
        return 0;
    }
    uint8_t overflow_mask = 0b10000000;
    for (uint8_t i = 1; i < steps; i++) {
        overflow_mask >>= 1;
        overflow_mask |= 0b10000000;
    }
    const uint8_t underflow_mask = 0b11111111 << steps;
    assert( !(src & (src - 1) ));  // Assert only a leading bit.
    // Steal brightness from brightness_src and give it to brightness_dst.
    // After this function concludes the multiplication of brightness_dst and brightness_src will remain
    // constant.
    uint8_t curr = *brightness_dst;
    uint8_t shifts = 0;
    for (uint8_t i = 0; i < max_shifts && (src & underflow_mask); i+=steps) {
        if (curr & overflow_mask) {
            // next shift will overflow
            break;
        }
        curr <<= steps;
        src >>= steps;
        shifts += steps;
    }
    // write out the output values.
    *brightness_dst = curr;
    *brightness_src = src;
    return shifts;
}

