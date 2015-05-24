// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#ifdef USE_TOMCRYPT
#include <tomcrypt.h>
#elif USE_OPENSSL
#include <openssl/des.h>
#else
#error "Define either USE_TOMCRYPT or USE_OPENSSL"
#endif

#include "brute.h"

#ifndef MAX_SIZE
#define MAX_SIZE 8
#endif

static uint8_t plaintext[MAX_SIZE] = {};
static uint8_t ciphertext[MAX_SIZE] = {};
static size_t byte_length = 0;
static int halt = 0;

int brute_init(const uint8_t *_plaintext, const uint8_t *_ciphertext, size_t _byte_length) {
  if (byte_length <= MAX_SIZE) {
    byte_length = _byte_length;
    memcpy(plaintext, _plaintext, byte_length);
    memcpy(ciphertext, _ciphertext, byte_length);
    return 1;
  } else {
    return 0;
  }
}

int64_t brute_search(const uint64_t key_from, const uint64_t key_to, uint64_t *keys_out) {
  const uint64_t *keys_start = keys_out;

  for (uint64_t key = key_from; key < key_to; key++) {
    unsigned char key_bytes[8];
    unsigned char local_plaintext[MAX_SIZE];

    // transform key to bytes, because we cannot rely on endianess
    for (int i = 0; i < 8; i++) {
      key_bytes[7 - i] = (key >> (i * 8)) & 0xff;
    }

#ifdef USE_TOMCRYPT
    symmetric_key k;
    des_setup(key_bytes, 8, 0, &k);
    des_ecb_decrypt(ciphertext, local_plaintext, &k);
    des_done(&k);
#elif USE_OPENSSL
    DES_key_schedule k;
    DES_set_key((const_DES_cblock *)key_bytes, &k);
    DES_ecb_encrypt((DES_cblock *)ciphertext, (DES_cblock *)local_plaintext, &k, DES_DECRYPT);
#endif

    // XXX: this is where an heuristics would kick in, to evaluate if local_plaintext is a possible plaintext message
    if (memcmp(local_plaintext, plaintext, byte_length) == 0) {
      *(keys_out++) = key;

      if (keys_out - keys_start >= MAX_KEYS) {
        fprintf(stderr, "\rWARN: No more space for new keys.\n");
        break;
      }
    }

    if (halt) {
      float percent = 100.f * (key - key_from) / (key_to - key_from);
      fprintf(stderr, "\rWARN: Halted search at %016"PRIx64": %6.2f%% of [%016"PRIx64", %016"PRIx64").\n", key, percent, key_from, key_to);
      break;
    }
  }

  return keys_out - keys_start;
}

void brute_halt(void) {
  halt = 1;
}
