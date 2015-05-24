// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "brute.h"
#include "util.h"

int main(int argc, char** argv) {
  uint64_t keys[256]; // = 0x3b3898371520f75e;
#if 0
  uint64_t key_from      = 0x3b38983715200000;
  uint64_t key_to        = 0x3b38983715210000;
#else
  uint64_t key_from     = 0x3b38983710000000;
  uint64_t key_to       = 0x3b38983800000000;
#endif

  uint64_t _pt = 0x1234123412341234;
  uint64_t _ct = 0xe56e427d61730001;
  uint8_t pt[8], ct[8];
  to_bytes(_pt, pt);
  to_bytes(_ct, ct);

  // the search
  {
    int matches;

    matches = brute_search(key_from, key_to, keys);

    if (matches > 0) {
      printf("matches:\n");
      for (int i = 0; i < matches; i++)
        printf("%016"PRIx64"\n", keys[i]);
    } else {
      printf("no match\n");
    }
  }

  return 0;
}
