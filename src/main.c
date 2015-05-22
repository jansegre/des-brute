// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <string.h>
#include <stdio.h>
#include "brute.h"

int main(int argc, char** argv) {
  unsigned char key[8];
#if 0
  unsigned char from_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0x00, 0x00};
  unsigned char to_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x21, 0x00, 0x00};
#else
  unsigned char from_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x00, 0x00, 0x00};
  unsigned char to_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x16, 0x00, 0x00, 0x00};
#endif

  unsigned char example_pt[] = {0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34};
  unsigned char example_et[] = {0xe5, 0x6e, 0x42, 0x7d, 0x61, 0x73, 0x00, 0x01};

  text_size = 8;
  memcpy(plain_text, example_pt, 8);
  memcpy(encoded_text, example_et, 8);

#if 0
  {
    unsigned char real_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
    symmetric_key skey;
    unsigned char my_pt[MAX_SIZE];
    des_setup(real_key, 8, 0, &skey);
    des_ecb_decrypt(encoded_text, my_pt, &skey);
    des_done(&skey);
    printstr(my_pt, 8);
  }
#endif

#if 1
  if (search(from_key, to_key, key)) {
    printf("key found\n");
    printstr(key, 8);
  } else {
    printf("not found\n");
  }
#endif

#if 0
  for (int i = 0; i < 500; i++) {
    printstr(from_key, 8);
    incr(from_key, 8);
  }
#endif

  return 0;
}
