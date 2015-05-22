// Copyright (c) 2015 Jan Segre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "brute.h"

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
  printf("Hello world from processor %s, rank %d"
      " out of %d processors\n",
      processor_name, world_rank, world_size);

  unsigned char key[8];
  unsigned char from_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x00, 0x00, 0x00};
  unsigned char to_key[8] = {0x3b, 0x38, 0x98, 0x37, 0x16, 0x00, 0x00, 0x00};

  unsigned char example_pt[] = {0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34};
  unsigned char example_et[] = {0xe5, 0x6e, 0x42, 0x7d, 0x61, 0x73, 0x00, 0x01};

  text_size = 8;
  memcpy(plain_text, example_pt, 8);
  memcpy(encoded_text, example_et, 8);

  if (search(from_key, to_key, key)) {
    printf("key found\n");
    printstr(key, 8);
  } else {
    printf("not found\n");
  }

  // Finalize the MPI environment.
  MPI_Finalize();

  return 0;
}
