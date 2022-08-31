#include <iostream>
#include <chrono>

#include "util.hpp"

#include "avl_buf_sort.hpp"
#include "rb_buf_sort.hpp"
#include "st_buf_sort.hpp"
#include "bv_buf_sort.hpp"
#include "bf_buf_sort.hpp"
#include "block_buf_sort.hpp"
#include "merge_inversions.hpp"

const static uint16_t BUFFER_SIZE = 1024;
const static uint32_t LIST_SIZE = uint32_t(1) << 14;

template<class DS>
double run_bench(B_type* buffer) {
    DS sorter;

    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    auto t1 = high_resolution_clock::now();
    sorter.sort(buffer);
    auto t2 = high_resolution_clock::now();
    for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
        std::cout << buffer[i].first << ", " << buffer[i].second << std::endl;
    }
    return duration_cast<microseconds>(t2 - t1).count();
}

double runner(uint64_t type, B_type* buffer) {
    if (type == 0) {
        return run_bench<AVL_tree<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else if (type == 1) {
        return run_bench<RB_tree<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else if (type == 2) {
        return run_bench<S_tree<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else if (type == 3) {
        return run_bench<BV_sorter<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else if (type == 4) {
        return run_bench<BF_sorter<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else if (type == 5) {
        return run_bench<Block_sort<BUFFER_SIZE, LIST_SIZE>>(buffer);
    } else {
        return run_bench<Merge_inversions<BUFFER_SIZE, LIST_SIZE>>(buffer);
    }
}

int main(int argc, char const *argv[])
{
    uint64_t type = 0;
    if (argc > 1) {
        std::sscanf(argv[1], "%lu", &type);
    }
    uint32_t n;
    std::cin >> n;

    B_type buffer[BUFFER_SIZE];
    //double time = 0;
    for (uint32_t i = 0; i < n; i++) {
        std::cout << "run " << i << std::endl;
        for (uint16_t j = 0; j < BUFFER_SIZE; j++) {
            std::cin >> buffer[j].first >> buffer[j].second;
        }
        double t = runner(type, buffer);
        //time += t;
        std::cerr << t << std::endl;
    }
    //std::cerr << "Mean time us: " << time / n << std::endl;
}
