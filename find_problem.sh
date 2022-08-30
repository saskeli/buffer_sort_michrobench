#!/bin/bash

source ~/venv/bin/activate

while : ; do
    python datagen.py 1 1024 > test.txt
    ./buf_sort 6 < test.txt > ms.txt
    ./buf_sort 4 < test.txt > bf.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
done