#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void* memset(void* ptr, int value, size_t len) {
  for (int i = 0; i < len; i++) {
    ((uint8_t*)ptr)[i] = value;
  }
  return ptr;
}

#endif // UTIL_H
