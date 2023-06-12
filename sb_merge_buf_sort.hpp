/*
Do quadratic sort of sub-blocks
Merge sub-blocks.
*/

#pragma once

#include <immintrin.h>

#include <algorithm>
#include <iostream>

#include "util.hpp"

template <uint16_t buffer_size, uint32_t n>
class SB_merge_inversions {
   private:
    inline static B_type scratch[buffer_size];
    B_type* buf = nullptr;

    static_assert(__builtin_popcount(buffer_size) == 1);

   public:
    void sort(B_type* buffer) {
        buf = buffer;
        if constexpr (buffer_size == 2) {
            twosort(buffer);
            return;
        }
        if constexpr (buffer_size == 4) {
            foursort(buffer);
            return;
        }
        if constexpr (buffer_size <= 128) {
            bf_sort<buffer_size>(buffer);
            return;
        }
        sort<buffer_size>(scratch, buffer);
    }

   private:
    template <uint16_t block_size>
    void sort(B_type* source, B_type* target) {
        if constexpr (block_size <= 64) {
            for (uint64_t i = 0; i < buffer_size; i += block_size) {
                bf_sort<block_size>(target + i);
            }
            // std::cerr << "BFSort " << block_size << ": " << std::endl;
            // print(buf);
            return;
        } else if constexpr (block_size == 128 &&
                             ((__builtin_clz(buffer_size / 16) & uint16_t(1)) == 0)) {
            for (uint64_t i = 0; i < buffer_size; i += block_size) {
                bf_sort<block_size>(target + i);
            }
            // std::cerr << "BFSort " << block_size << ": " << std::endl;
            // print(buf);
            return;
        } else {
            const constexpr uint16_t sub_block_size = block_size / 2;
            sort<sub_block_size>(target, source);
            for (uint16_t offset = 0; offset < buffer_size;
                 offset += block_size) {
                uint16_t inversions = 0;
                uint16_t a_idx = offset;
                uint16_t b_idx = offset + sub_block_size;
                uint16_t a_limit = b_idx;
                uint16_t b_limit = offset + block_size;
                for (uint16_t i = 0; i < block_size; i++) {
                    if (a_idx < a_limit) [[likely]] {
                        if (b_idx < b_limit) [[likely]] {
                            // std::cerr
                            //     << "s_a(" << a_idx << ") + inv < s_b(" << b_idx
                            //     << ") = " << source[a_idx] << " + "
                            //     << inversions << " < " << source[b_idx] << " = "
                            //     << (source[a_idx] + inversions < source[b_idx])
                            //     << std::endl;
                            if (source[a_idx] + inversions < source[b_idx]) {
                                target[offset + i] =
                                    source[a_idx++] + inversions;
                            } else {
                                target[offset + i] = source[b_idx++];
                                inversions++;
                            }
                        } else {
                            target[offset + i] = source[a_idx++] + inversions;
                        }
                    } else {
                        target[offset + i] = source[b_idx++];
                    }
                }
            }
            // std::cerr << "Merge to " << block_size << ": " << std::endl;
            // print(buf);
        }
    }

    void twosort(B_type* source) {
        for (uint16_t i = 0; i < buffer_size; i += 2) {
            if (source[i + 1] <= source[i]) {
                ++source[i];
                std::swap(source[i], source[i + 1]);
            }
        }
        // std::cerr << "two sort:" << std::endl;
        // print(buf);
    }

    void foursort(B_type* source) {
        for (uint16_t i = 0; i < buffer_size; i += 4) {
            source[i] += uint32_t(source[i + 1] <= source[i]);

            source[i + 1] += uint32_t(source[i + 2] <= source[i + 1]);
            source[i] += uint32_t(source[i + 2] <= source[i]);

            source[i + 2] += uint32_t(source[i + 3] <= source[i + 2]);
            source[i + 1] += uint32_t(source[i + 3] <= source[i + 1]);
            source[i] += uint32_t(source[i + 3] <= source[i]);
            if (source[i + 2] > source[i + 3]) {
                std::swap(source[i + 2], source[i + 3]);
            }
            if (source[i] > source[i + 1]) {
                std::swap(source[i], source[i + 1]);
            }
            if (source[i] > source[i + 2]) {
                std::swap(source[i], source[i + 2]);
            }
            if (source[i + 1] > source[i + 3]) {
                std::swap(source[i + 1], source[i + 3]);
            }
            if (source[i + 1] > source[i + 2]) {
                std::swap(source[i + 1], source[i + 2]);
            }
        }
        // std::cerr << "Four sort:" << std::endl;
        // print(buf);
    }

    template <uint16_t size>
    void bf_sort(B_type* source) {
#pragma GCC unroll 512
        for (uint16_t i = 1; i < size; i++) {
            for (uint16_t j = 0; j < i; j++) {
                bool smaller = source[i] <= source[j];
                source[j] += smaller;
            }
        }
        std::sort(source, source + size);
    }

    /*
    1, 2, 3, 4, 5, 3 =>
    1, 2, 3, 4, 5, 6
    scoot = 3
    i = 5

    */

    void print(B_type* buffer) {
        std::cerr << "buffer:" << std::endl;
        for (uint16_t i = 0; i < buffer_size; i++) {
            std::cerr << buffer[i] << " ";
            if (i % 4 == 3) {
                std::cerr << std::endl;
            }
        }
        std::cerr << "scratch:" << std::endl;
        for (uint16_t i = 0; i < buffer_size; i++) {
            std::cerr << scratch[i] << " ";
            if (i % 4 == 3) {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }
};