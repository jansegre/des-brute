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
#include <stdlib.h>
#include <inttypes.h>
#include <getopt.h>
#include <time.h>

#include "brute.h"
#include "util.h"

void sigint_handler(int sig) {
  brute_halt();
}

int main(int argc, char** argv) {
  size_t text_size = 8;
  uint8_t plaintext[1024], ciphertext[1024], keyfrom[8], keyto[8];

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"plaintext",  required_argument, 0, 'p'},
      {"ciphertext", required_argument, 0, 'c'},
      {"from",       required_argument, 0, 'f'},
      {"to",         required_argument, 0, 't'},
      {"verbose",    no_argument,       0, 'v'},
      {}
    };

    int s, c = getopt_long(argc, argv, "p:c:f:t:v", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 'p':
        s = from_hex(plaintext, optarg);
        if (s < 0) {
          fprintf(stderr, "Invalid hex for plaintext\n");
          return EXIT_FAILURE;
        }
        text_size = min(min(s, 8), text_size);
        break;

      case 'c':
        s = from_hex(ciphertext, optarg);
        if (s < 0) {
          fprintf(stderr, "Invalid hex for ciphertext\n");
          return EXIT_FAILURE;
        }
        text_size = min(min(s, 8), text_size);
        break;

      case 'f':
        s = from_hex(keyfrom, optarg);
        if (s < 0) {
          fprintf(stderr, "Invalid hex for keyfrom\n");
          return EXIT_FAILURE;
        }
        if (s != 8) {
          fprintf(stderr, "Invalid key size, must have 8 bytes\n");
          return EXIT_FAILURE;
        }
        break;

      case 't':
        s = from_hex(keyto, optarg);
        if (s < 0) {
          fprintf(stderr, "Invalid hex for keyto\n");
          return EXIT_FAILURE;
        }
        if (s != 8) {
          fprintf(stderr, "Invalid key size, must have 8 bytes\n");
          return EXIT_FAILURE;
        }
        break;

      case 'v':
        printf("enable verbose\n");

      case '?':
        break;

      default:
        fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
        abort();
    }
  }

  if (optind < argc) {
    fprintf(stderr, "These options were not understood: ");
    while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\n");
    return EXIT_FAILURE;
  }
  //uint64_t keys[MAX_KEYS]; // = 0x3b3898371520f75e;
  //uint64_t key_from           = 0x3b38983715000000;
  //uint64_t key_to             = 0x3b38983716000000;
  uint64_t keys[MAX_KEYS];
  uint64_t key_from = from_bytes(keyfrom);
  uint64_t key_to = from_bytes(keyto);

  if (key_from > key_to) {
    fprintf(stderr, "Invalid search space: %016"PRIx64" > %016"PRIx64"\n", key_from, key_to);
    return EXIT_FAILURE;
  }

  // Initialize the search
  brute_init(plaintext, ciphertext, text_size);

  // Handle SIGINT (Ctrl+C)
  {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
  }

  printf("plaintext:   %s\n", to_hex(plaintext, text_size));
  printf("ciphertext:  %s\n", to_hex(ciphertext, text_size));
  printf("search from: %016"PRIx64"\n", key_from);
  printf("search to:   %016"PRIx64"\n", key_to);
  printf("search space: ");
  double bits = log2((double)(key_to - key_from));
  if (key_from == key_to) printf("no bits");
  else if (is2pow(key_to - key_from)) printf("%.0lf bits", bits);
  else printf("~%.1lf bits", bits);
  printf("\n");

  clock_t total_ticks = -clock();

  // Local search, **this is the heavy part**
  int matches = brute_search(key_from, key_to, keys);

  total_ticks += clock();
  double total_time = (double)(total_ticks) / CLOCKS_PER_SEC;

  switch (matches) {
    case 0: printf("no matches in %lf seconds.\n", total_time); break;
    case 1: printf("1 match in %lf seconds:\n", total_time); break;
    default: printf("%i matches in %lf seconds:\n", matches, total_time); break;
  }

  for (int i = 0; i < matches; i++)
    printf("%016"PRIx64"\n", keys[i]);

  return EXIT_SUCCESS;
}
