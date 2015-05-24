// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

#define _XOPEN_SOURCE 700
// http://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error
#include <signal.h>
#undef _XOPEN_SOURCE
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "brute.h"
#include "util.h"

void sigint_handler(int sig) {
  brute_halt();
}

int main(int argc, char** argv) {
  uint64_t keys[MAX_KEYS]; // = 0x3b3898371520f75e;
  uint64_t key_from           = 0x3b38983710000000;
  uint64_t key_to             = 0x3b38983800000000;

  uint64_t pt = 0x1234123412341234;
  uint64_t ct = 0xe56e427d61730001;
  size_t text_size = 8;
  uint8_t plaintext[text_size], ciphertext[text_size];
  to_bytes(pt, plaintext);
  to_bytes(ct, ciphertext);
  brute_init(plaintext, ciphertext, text_size);

  // Handle SIGINT (Ctrl+C)
  {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
  }

  printf("Plaintext:  ");
  print_hex_text(plaintext, text_size);
  puts("");
  printf("Ciphertext: ");
  print_hex_text(ciphertext, text_size);
  puts("");
  double bits = log2((double)(key_to - key_from));
  printf("[%016"PRIx64", %016"PRIx64") search space (", key_from, key_to);
  if (is2pow(key_to - key_from)) printf("%.0lfbits", bits);
  else printf("~%.1lfbits", bits);
  printf(")\n");

  // Local search, **this is the heavy part**
  int matches = brute_search(key_from, key_to, keys);

  switch (matches) {
    case 0: printf("No matches.\n"); break;
    case 1: printf("1 match:\n"); break;
    default: printf("%i matches:\n", matches); break;
  }

  for (int i = 0; i < matches; i++)
    printf("%016"PRIx64"\n", keys[i]);

  return 0;
}
