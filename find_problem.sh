#!/bin/bash

source ~/venv/bin/activate

while : ; do
    python datagen.py 1 64 > test.txt
    ./buf_sort 5 < test.txt > bs.txt
    ./buf_sort 4 < test.txt > bf.txt
    diff bs.txt bf.txt
    [[ $? == 0 ]] || break
done