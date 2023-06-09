#pragma once

#include <algorithm>
#include <iostream>

#include "util.hpp"

template<uint16_t buffer_size, uint32_t leaf_size>
class BF_sorter {
  public:
    void sort(B_type* buffer) {
#pragma GCC unroll 1024
        for (uint16_t i = 1; i < buffer_size; i++) {
            for (uint16_t j = 0; j < i; j++) {
                buffer[j] += buffer[i] <= buffer[j];
            }
        }
        std::sort(buffer, buffer + buffer_size);
    }
};