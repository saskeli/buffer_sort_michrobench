#!/bin/bash

source ~/venv/bin/activate

for b in 8 16 32 64 128 256 512 1024
do
    g++ -std=c++2a -Wall -Wextra -Wshadow -pedantic -march=native -DSET_BUFFER_SIZE=${b} -DNDEBUG -Ofast -o buf_sort buf_sort.cpp
    python datagen.py 100000 ${b} > test.txt
    ./buf_sort 0 < test.txt 2> res/AVL_${b}.txt >> /dev/null
    ./buf_sort 1 < test.txt 2> res/RB_${b}.txt >> /dev/null
    ./buf_sort 2 < test.txt 2> res/BV-ST_${b}.txt >> /dev/null
    ./buf_sort 3 < test.txt 2> res/BV-BT_${b}.txt >> /dev/null
    ./buf_sort 4 < test.txt 2> res/BF_${b}.txt >> /dev/null
    ./buf_sort 5 < test.txt 2> res/BLOCK_${b}.txt >> /dev/null
    ./buf_sort 6 < test.txt 2> res/MS_${b}.txt >> /dev/null
    ./buf_sort 7 < test.txt 2> res/BMS_${b}.txt >> /dev/null
done