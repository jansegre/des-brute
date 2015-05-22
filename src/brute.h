// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stddef.h>

#define MAX_SIZE 16

extern unsigned char plain_text[MAX_SIZE];
extern unsigned char encoded_text[MAX_SIZE];
extern size_t text_size;
extern int halt;

void incr(unsigned char *a, size_t len);
int search(const unsigned char *from, const unsigned char *to, unsigned char *key_out);
void printstr(unsigned char *str, int len);
