#ifndef PRINT_H
#define PRINT_H

#include <stddef.h>

void print(void*, size_t);

size_t strlen(const char * str) {
  size_t len = 0;
  while (*str != '\0') {
    len++;
    str++;
  }
  return len;
}

void printstr(char* str) {
  print(str, strlen(str));
}

#endif // PRINT_H
