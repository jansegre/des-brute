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
#include <mpi.h>

#include "brute.h"
#include "util.h"

void sigint_handler(int sig) {
  brute_halt();
}

void print_hex_text(const uint8_t *text, size_t size) {
  for (int i = 0; i < size; i++) {
    printf("%02x", text[i]);
  }
}

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  uint64_t keys[MAX_KEYS]; // = 0x3b3898371520f75e;
  uint64_t key_from           = 0x3b38983715000000;
  uint64_t key_to             = 0x3b38983716000000;

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
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
  }

  // Show master information
  if (world_rank == 0) {
    printf("Plaintext:  ");
    print_hex_text(plaintext, text_size);
    puts("");
    printf("Ciphertext: ");
    print_hex_text(ciphertext, text_size);
    puts("");
    if (world_size > 1)
      printf("[%016"PRIx64", %016"PRIx64") search space splitted on %i processor(s):\n", key_from, key_to, world_size);
  }

  uint64_t key_slice = (key_to - key_from) / world_size;
  uint64_t my_key_from = key_from + key_slice * world_rank;
  uint64_t my_key_to = key_from + key_slice * (world_rank + 1);
  if (world_rank == world_size - 1)
    my_key_to = key_to;

  // Print off a hello world message
  MPI_Barrier(MPI_COMM_WORLD);
  printf("[%016"PRIx64", %016"PRIx64") search space on %s %i\n", my_key_from, my_key_to, processor_name, world_rank);

  // the search
  {
    double total_time = 0.0;
    int matches;

    // start timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_time -= MPI_Wtime();

    matches = brute_search(my_key_from, my_key_to, keys);

    // end timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_time += MPI_Wtime();
    printf("Total time: %lf seconds on %s %i\n", total_time, processor_name, world_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    int total_matches = matches;
    if (world_rank == 0) {
      for (int i = 1; i < world_size; i++) {
        int other_matches = 0;
        MPI_Recv(&other_matches, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_matches += other_matches;
      }
    } else {
      MPI_Send(&matches, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (world_rank == 0) {
      switch (total_matches) {
        case 0: printf("No matches.\n"); break;
        case 1: printf("1 match:\n"); break;
        default: printf("%i matches:\n", total_matches); break;
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    //printf("DEBUG: %i matches on %s %i\n", matches, processor_name, world_rank);
    for (int i = 0; i < matches; i++)
      printf("%016"PRIx64"\n", keys[i]);
  }

  // Finalize the MPI environment.
  MPI_Finalize();

  return 0;
}
