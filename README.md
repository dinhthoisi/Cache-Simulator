# Cache-Simulator
This project implements cache simulator by C++. It also simulates multi-word blocks (n = 2), multi-level cache (L1 and L2) and classifying cache MISS

The working environment is Linux

There are some test files : input0.txt, input1.txt, input2.txt, and input3.txt

To compile main.cpp, run g++ -o cache_sim main.cpp -std=c++11, so we have the executable file cache_sim

For testing :

command-line arguments are: ./cache_sim num_entries associativity num_words memory_reference_file

num_words is for multi-words block and this project works for num_words = 1 or num_words = 2

For input0.txt, run ./cache_sim 4 2 1 input0.txt

output file is input0.txt_output, to view input0.txt_output, run vi input0.txt_output. The ouput file also show MISS types

To simulate multiple caches, run ./cache_sim 4 2 1 8 2 1 input0.txt

4 2 1 is for L1 and 8 2 1 is for L2
