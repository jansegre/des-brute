// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <mpi.h>

#include "brute.h"
#include "util.h"

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

  // Print off a hello world message
  printf("Hello world from processor %s, rank %d out of %d processors\n",
    processor_name, world_rank, world_size);

  uint64_t keys[256]; // = 0x3b3898371520f75e;
  uint64_t key_from      = 0x3b38983710000000;
  uint64_t key_to        = 0x3b38983720000000;

  uint64_t _pt = 0x1234123412341234;
  uint64_t _ct = 0xe56e427d61730001;
  uint8_t pt[8], ct[8];
  to_bytes(_pt, pt);
  to_bytes(_ct, ct);

  // the search
  { 
    double total_time = 0.0;
    int matches;
   
    brute_init(pt, ct, 8);

    // start timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_time -= MPI_Wtime();

    matches = brute_search(key_from, key_to, keys);

    // end timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_time += MPI_Wtime();

    printf("Total time: %lf (proc %s, rank %d)\n",
      total_time, processor_name, world_rank);

    if (matches > 0) {
      printf("matches:\n");
      for (int i = 0; i < matches; i++)
        printf("%016"PRIx64"\n", keys[i]);
    } else {
      printf("no match here\n");
    }
  }

  // Finalize the MPI environment.
  MPI_Finalize();

  return 0;
}
