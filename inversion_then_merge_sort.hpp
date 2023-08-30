
#pragma once

#include <immintrin.h>
#include <array>

#include <algorithm>
#include <iostream>

#include "util.hpp"

template <uint16_t buffer_size, uint32_t n>
class inversion_then_merge_sort {
   private:
    static const constexpr uint32_t bf_limit = 16;
    inline static B_type scratch[n];
    B_type* buf = nullptr;

    static_assert(__builtin_popcount(buffer_size) == 1);

   public:
    void sort(B_type* buffer) {
        buf = buffer;
        //print();
        sort<buffer_size, true>(buf, scratch);
    }

   private:
    template<uint16_t init, uint16_t step, uint16_t size>
    constexpr std::array<uint16_t, size> init_arr() {
        std::array<uint16_t, size> ret;
        for (uint16_t i = 0; i < size; i++) {
            ret[i] = i * step + init;
        }
        return ret;
    }

    template <uint16_t block_size, bool in_target>
    void sort(B_type target[], B_type source[]) {
        if constexpr (block_size <= bf_limit && in_target) {
            for (uint64_t i = 0; i < buffer_size; i += block_size) {
                bf_sort<block_size>(buf + i);
            }
            //std::cerr << "After brute force(" << block_size << ") sort." << std::endl;
            //print(buf);
        } else {
            const constexpr uint16_t sub_block_size = block_size / 2;
            const constexpr uint16_t blocks = buffer_size / block_size;
            sort<sub_block_size, !in_target>(source, target);

            auto a_s(init_arr<0, block_size, blocks>());
            auto a_e(init_arr<sub_block_size - 1, block_size, blocks>());
            auto b_s(init_arr<sub_block_size, block_size, blocks>());
            auto b_e(init_arr<block_size - 1, block_size, blocks>());
            auto s_inv(init_arr<0, 0, blocks>());
            auto e_inv(init_arr<sub_block_size, 0, blocks>());

            for (uint16_t i = 0; i < block_size / 2; ++i) {
                for (uint16_t j = 0; j < blocks; ++j) {
                    if (a_s[j] <= a_e[j]) [[likely]] {
                        auto pot = source[a_s[j]] + s_inv[j];
                        if (b_s[j] <= b_e[j]) [[likely]] {
                            if (pot < source[b_s[j]]) {
                                target[j * block_size + i] = pot;
                                ++a_s[j];
                            } else {
                                target[j * block_size + i] = source[b_s[j]++];
                                ++s_inv[j];
                            }
                        } else {
                            target[j * block_size + i] = pot;
                            ++a_s[j];
                        }
                    } else {
                        target[j * block_size + i] = source[b_s[j]++];
                    }

                    if (a_e[j] >= a_s[j]) [[likely]] {
                        auto pot = source[a_e[j]] + e_inv[j];
                        if (b_e[j] >= b_s[j]) [[likely]] {
                            if (pot > source[b_e[j]]) {
                                target[j * block_size + block_size - i - 1] = pot;
                                --a_s[j];
                            } else {
                                target[j * block_size + block_size - i - 1] = source[b_e[j]--];
                                --e_inv[j];
                            }
                        } else {
                            target[j * block_size + block_size - i - 1] = pot;
                            --a_s[j];
                        }
                    } else {
                        target[j * block_size + block_size - i - 1] = source[b_e[j]--];
                    }
                }
            }
        }
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

    void print(B_type* source) {
        std::cerr << "buffer:" << std::endl;
        for (uint16_t i = 0; i < buffer_size; i++) {
            std::cerr << source[i] << " ";
            if (i % 4 == 3) {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }
};