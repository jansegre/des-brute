// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
#ifndef UTIL_H
#define UTIL_H

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

#endif
