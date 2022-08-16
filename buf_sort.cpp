#include <iostream>
#include <chrono>

#include "avl_buf_sort.hpp"

typedef std::pair<uint32_t, uint32_t> B_type;

const static uint16_t BUFFER_SIZE = 1024;
const static uint32_t LIST_SIZE = 1 << 14;

template<class DS>
double run_bench(B_type* buffer) {
    DS sorter;

    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    auto t1 = high_resolution_clock::now();
    for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
        sorter.insert(buffer[i].first, buffer[i].second);
        //std::cerr << buffer[i].first << ", " << buffer[i].second << " inserted" << std::endl;
        //sorter.print();
    }

    auto* mapping = sorter.get_mapping();
    auto t2 = high_resolution_clock::now();

    for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
        std::cout << mapping[i].first << ", " << mapping[i].second << std::endl;
    }
    return duration_cast<microseconds>(t2 - t1).count();
}

int main() {
    uint32_t n;
    std::cin >> n;

    B_type buffer[BUFFER_SIZE];
    double time = 0;
    for (uint32_t i = 0; i < n; i++) {
        for (uint16_t j = 0; j < BUFFER_SIZE; j++) {
            std::cin >> buffer[j].first >> buffer[j].second;
        }
        time += run_bench<AVL_tree<BUFFER_SIZE>>(buffer);
    }
    std::cerr << "Mean time us: " << time / n << std::endl;
}