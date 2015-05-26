# des-brute

Distributed DES brute-force cracking.

This is a joint assignment of Distributed Systems and Information Security
courses at [IME](http://ime.eb.br/) from 2015.

## Requirements

- OpenSSL (libcrypto)
- OpenMPI [optional]

## Usage

Compiling is as easy as

    make

Running requires some a plaintext (-p), a ciphertext (-c) and a key search space (-f from, -t to).

For example,

    mpirun -np 16 brute_mpi -p 1234123412341234 -c 57475a4f836b31d5 -f 5678567800000000 -t 5678567900000000

And the output should look like this:

    plaintext:   1234123412341234
    ciphertext:  57475a4f836b31d5
    search from: 5678567800000000
    search to:   5678567900000000
    search space: 32 bits
    search splitted across 16 processor(s):
    [5678567800000000, 5678567810000000) search space on nid00024 0
    [5678567810000000, 5678567820000000) search space on nid00024 1
    [5678567830000000, 5678567840000000) search space on nid00024 3
    [5678567890000000, 56785678a0000000) search space on nid00024 9
    [5678567820000000, 5678567830000000) search space on nid00024 2
    [5678567850000000, 5678567860000000) search space on nid00024 5
    [56785678f0000000, 5678567900000000) search space on nid00024 15
    [5678567840000000, 5678567850000000) search space on nid00024 4
    [5678567880000000, 5678567890000000) search space on nid00024 8
    [5678567860000000, 5678567870000000) search space on nid00024 6
    [56785678b0000000, 56785678c0000000) search space on nid00024 11
    [5678567870000000, 5678567880000000) search space on nid00024 7
    [56785678d0000000, 56785678e0000000) search space on nid00024 13
    [56785678e0000000, 56785678f0000000) search space on nid00024 14
    [56785678c0000000, 56785678d0000000) search space on nid00024 12
    [56785678a0000000, 56785678b0000000) search space on nid00024 10
    16 matches in 158.890091 seconds:
    5678567856785678
    5678567856785679
    5678567856785778
    5678567856785779
    5678567856795678
    5678567856795679
    5678567856795778
    5678567856795779
    5678567857785678
    5678567857785679
    5678567857785778
    5678567857785779
    5678567857795678
    5678567857795679
    5678567857795778
    5678567857795779

The search space is a range, it is not limited to an integer number of bits.


## Benchmark

Though not tailored for performance this code was benchmarked on a Cray XK7,
unfortunately using only 16 processes.

These are the results:

![Linear Time][image_eng.jpg]

![Logarithmic Time][image_log_eng.jpg]

## License

Distributed under Mozilla Public License 2.0
