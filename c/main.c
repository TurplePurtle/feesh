// Articles used as reference:
// https://dassur.ma/things/c-to-webassembly/
// https://aransentin.github.io/cwasm/

#define export __attribute__((visibility("default")))

#include <stdint.h>
#include <stddef.h>
#include "draw.h"
#include "fish1.h"
#include "util.h"

void flush_frame();

export uint16_t* get_frame_buffer() {
  return img_buffer;
}

static uint16_t frame_count = 0;
static FishState fishies[] = {
  {
    .x = 10,
    .y = 10,
    .velx = 20,
    .vely = 20,
    .time_born = 0,
    .type = &fish1_type,
    .current_frame = 0,
    .current_frame_progress = 0,
    .facing_direction = 0,
  },
  {
    .x = 100,
    .y = 20,
    .velx = -20,
    .vely = 18,
    .time_born = 0,
    .type = &fish1_type,
    .current_frame = 0,
    .current_frame_progress = 0,
    .facing_direction = 0,
  },
};

export void init() {
  fishies[0].current_animation = fishies[0].type->anim_idle;
  fishies[1].current_animation = fishies[1].type->anim_idle;
}

void update() {
  for (size_t i = 0; i < arrlen(fishies); i++) {
    fish_update_pos(&fishies[i]);
    fish_update_frame(&fishies[i]);
  }
}

void draw() {
  memset(img_buffer, 0, sizeof(img_buffer));

  for (size_t i = 0; i < arrlen(fishies); i++) {
    FishState* fish = &fishies[i];
    const uint16_t* fish_palette = fish->type->palette;
    uint16_t fish_width = fish->type->width;
    uint16_t fish_height = fish->type->height;
    for (int16_t y = 0; y < fish_height; y++) {
      int16_t out_y = fish->y + y;
      if (out_y < 0 || out_y >= HEIGHT) continue;
      for (int16_t x = 0; x < fish_width; x++) {
        int16_t out_x = fish->facing_direction == 0 ?
          fish->x + x :
          fish->x + (fish->type->width - x);
        if (out_x < 0 || out_x >= WIDTH) continue;

        uint8_t frame_index =
          fish->current_animation->frame_start_index + fish->current_frame;
        size_t frame_start =
          (size_t)frame_index * (size_t)fish_width * (size_t)fish_height;
        uint8_t color_index = fish->type->frames[frame_start + y * fish_width + x];
        if (color_index != 0) {
          uint16_t color = fish_palette[color_index];
          set_pixel16(out_x, out_y, color);
        }
      }
    }
  }
}

export void on_frame() {
  frame_count++;
  update();
  draw();
  flush_frame();
}
