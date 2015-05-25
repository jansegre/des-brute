// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <math.h>

void to_bytes(uint64_t from, uint8_t to[8]) {
    for (int i = 0; i < 8; i++) {
      to[7 - i] = (from >> (i * 8)) & 0xff;
    }
}

uint64_t from_bytes(uint8_t from[8]) {
  uint64_t to = 0;
  for (int i = 0; i < 8; i++) {
    to = (to << 8) | from[i];
  }
  return to;
}

int is2pow(int64_t x) {
  return x && !(x & (x - 1)) != 0;
}

const char * to_hex(const uint8_t *text, size_t size) {
  static char hex[1024];
  int i;
  for (i = 0; i < size; i++) {
    sprintf(hex + 2 * i, "%02x", text[i]);
  }
  hex[2 * i] = 0;
  return hex;
}

int from_hex(uint8_t *bytes, char *hex) {
  int len = strlen(optarg);
  if (len % 2 != 0) {
    return -1;
  }

  int text_size = len / 2;
  for(int i = 0; i < text_size; i++) {
    sscanf(hex + i * 2, "%2hhx", bytes + i);
  }

  return text_size;
}

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
