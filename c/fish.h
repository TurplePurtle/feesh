#ifndef FISH_H
#define FISH_H

#include <stdint.h>
#include "draw.h"
#include "fish1.h"

#define arrlen(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct FishAnimation {
  uint8_t frame_start_index;
  uint8_t frame_count;
} FishAnimation;

typedef struct FishType {
  uint8_t width;
  uint8_t height;
  uint8_t num_frames;
  const uint8_t* frames;
  const FishAnimation* anim_idle;
  uint8_t video_frames_per_sprite_frame;
  const uint16_t* palette;
  uint8_t palette_length;
} FishType;

typedef struct FishState {
  float x;
  float y;
  float velx;
  float vely;
  uint32_t time_born;
  const FishType* type;
  const FishAnimation* current_animation;
  uint8_t current_frame;
  uint8_t current_frame_progress;
  uint8_t facing_direction;
  uint8_t current_behavior;
  uint16_t targetx;
  uint16_t targety;
  float maxvel;
  float minvel;
  uint32_t delay_time;
} FishState;

inline void fish_update_pos(FishState* fish) {
  float dt = 16. / 1000;
  if ((fish->velx > 0 && (fish->x + fish->type->width) >= WIDTH) ||
      (fish->velx < 0 && fish->x <= 0)) {
    fish->velx = -fish->velx;
  }
  if ((fish->vely > 0 && (fish->y + fish->type->height) >= HEIGHT) ||
      (fish->vely < 0 && fish->y <= 0)) {
    fish->vely = -fish->vely;
  }
  fish->x += fish->velx * dt;
  fish->y += fish->vely * dt;
}

inline void fish_update_frame(FishState* fish) {
  fish->current_frame_progress =
    (fish->current_frame_progress + 1) % fish->type->video_frames_per_sprite_frame;
  if (fish->current_frame_progress == 0) {
    fish->current_frame =
      (fish->current_frame + 1) % fish->current_animation->frame_count;
  }
}

#endif // FISH_H
