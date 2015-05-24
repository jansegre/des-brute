// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
#ifndef BRUTE_H
#define BRUTE_H

#include <stddef.h>
#include <stdint.h>

/// Fill in the plaintext and ciphertext to use for the search, non-zero on success
int brute_init(const uint8_t *plaintext, const uint8_t *ciphertext, size_t byte_length);

/// Searches for a key in the space [key_from, key_to)
/// Return the number of matches, write each one to key_out in sequence
int64_t brute_search(uint64_t key_from, uint64_t key_to, uint64_t *keys_out);

/// Will stop the search as soon as possible
void brute_halt(void);

#endif
