#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

#define WIDTH 320
#define HEIGHT 240
#define FRAME_SIZE (WIDTH * HEIGHT)
uint16_t img_buffer[FRAME_SIZE];

inline void set_pixel8(int16_t x, int16_t y, uint8_t r8, uint8_t g8, uint8_t b8) {
  uint16_t r = (((uint16_t)r8) << 8) & 0b1111100000000000;
  uint16_t g = (((uint16_t)g8) << 3) & 0b0000011111100000;
  uint16_t b = (((uint16_t)b8) >> 3) & 0b0000000000011111;
  img_buffer[y * WIDTH + x] = b | g | r;
}

inline void set_pixel16(int16_t x, int16_t y, uint16_t val) {
  img_buffer[y * WIDTH + x] = val;
}

#endif // DRAW_H
