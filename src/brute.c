// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdio.h>
#include <string.h>
#ifdef USE_TOMCRYPT
#include <tomcrypt.h>
#elif USE_OPENSSL
#include <openssl/des.h>
#else
#error "Define either USE_TOMCRYPT or USE_OPENSSL"
#endif

#define MAX_SIZE 1024

unsigned char plain_text[MAX_SIZE] = {};
unsigned char encoded_text[MAX_SIZE] = {};
size_t text_size = 0;
int halt = 0;

/// Increments a byte vector, a[0] is the most significant byte, a[len-1] is the least
void incr(unsigned char *a, size_t len) {
  while (len--) if (++a[len]) break;
}

/// Searches for a key in the space [from, to)
/// If there's a match, key is written and 1 is returned, 0 otherwise
int search(const unsigned char *from, const unsigned char *to, unsigned char *key_out) {
  unsigned char key_now[8];
  unsigned char pt[MAX_SIZE];
  memcpy(key_now, from, 8);
  while (!halt && memcmp(key_now, to, 8) < 0) {
#ifdef USE_TOMCRYPT
    symmetric_key key;
    des_setup(key_now, 8, 0, &key);
    des_ecb_decrypt(encoded_text, pt, &key);
    des_done(&key);
#elif USE_OPENSSL
    DES_cblock key;
    memcpy(key, key_now, 8);
    DES_key_schedule keysched;
    DES_set_key(&key, &keysched);
    DES_ecb_encrypt((DES_cblock *)encoded_text, (DES_cblock *)pt, &keysched, DES_DECRYPT);
#endif
    // XXX: this is where an heuristics would kick in, to evaluate if pt is a plain_text
    if (!memcmp(pt, plain_text, text_size)) {
      memcpy(key_out, key_now, 8);
      return 1;
    }
    incr(key_now, 8);
  }
  return 0;
}

void printstr(unsigned char *str, int len) {
  int i;
  //printf("ascii: %s\n", str);
  printf("hex: ");
  for (i = 0; i < len; i++) {
    unsigned char c = str[i];
    printf("%02x", c);
  }
  printf("\n");
}

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
