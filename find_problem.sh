#!/bin/bash

source ~/venv/bin/activate

while : ; do
    python datagen.py 1 512 > test.txt
    ./buf_sort 0 < test.txt > ms.txt
    ./buf_sort 7 < test.txt > bf.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 1 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 2 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 3 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 4 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 5 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    ./buf_sort 6 < test.txt > ms.txt
    diff ms.txt bf.txt
    [[ $? == 0 ]] || break
    echo ******passed*******
done