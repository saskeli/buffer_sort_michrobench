#pragma once

#include <algorithm>
#include <iostream>
#include <immintrin.h>
#include <cstring>
#include <bitset>

#include "util.hpp"

template<uint16_t size, uint32_t n>
class S_tree {
  private:
    static const constexpr uint64_t WORD_BITS = 64;
    static const constexpr uint64_t WORDS = n / WORD_BITS;
    inline static uint64_t data[WORDS];
    inline static uint16_t segment_tree[WORDS];

    static_assert(__builtin_popcountll(n) == 1);
    static_assert(n > 64);

  public:
    void sort(B_type* buffer) {
        memset(data, 0, WORDS * sizeof(uint64_t));
        memset(segment_tree, 0, WORDS * sizeof(uint16_t));
        for (uint16_t i = size - 1; i < size; i--) {
            //bool print_b = false;
            //if (buffer[i].first == 16359) {
            //    print();
            //    print_b = true;
            //}
            buffer[i].first += update(buffer[i].first);
            //if (print_b) {
            //    print();
            //}
        }
        std::sort(buffer, buffer + size);
    }

    void print() {
        uint32_t idx = 1;
        for (uint32_t e = 1; e <= WORDS / 2; e *= 2) {
            for (uint32_t i = 0; i < e; i++) {
                std::cerr << segment_tree[idx++] << " ";
            }
            std::cerr << std::endl;
        }
        for (uint32_t i = 0; i < WORDS; i++) {
            std::cerr << std::bitset<64>(data[i]) << " " << i * 64 << std::endl;
        }
    }

  private:

    uint32_t update(uint32_t index) {
        //std::cerr << "update(" << index << ")" << std::endl;
        uint32_t idx = 1;
        uint32_t block_elems = n / 2;
        uint32_t cumulative_sum = 0;
        while (idx < WORDS) {
            uint32_t block_zeros = block_elems - segment_tree[idx];
            //std::cerr << " Looking at " << idx << " with " << block_zeros << " zeros to the left <-> " << index << std::endl;
            if (block_zeros <= index) {
                //std::cerr << " Going right" << std::endl;
                cumulative_sum += segment_tree[idx];
                idx = 2 * idx + 1;
                index -= block_zeros;
                block_elems /= 2;
            } else {
                //std::cerr << " Going left" << std::endl;
                idx = 2 * idx;
                block_elems /= 2;
            }
            //std::cerr << " cumulative sum: " << cumulative_sum << std::endl;
        }
        uint32_t word_idx = idx - WORDS;
        //std::cerr << "Word index " << word_idx << std::endl;
        uint16_t word_offset = 63 - __builtin_clzll(_pdep_u64(uint64_t(1) << index, ~data[word_idx]));
        uint64_t set_bit = uint64_t(1) << word_offset;
        //std::cerr << "Want the " << index << "-th zero. Should be at index " << word_offset << std::endl;
        //std::cerr << "  " << std::bitset<64>(data[word_idx]) << std::endl;
        //std::cerr << "& " << std::bitset<64>(set_bit - 1) << std::endl;
        cumulative_sum += __builtin_popcountll(data[word_idx] & (set_bit - 1));
        //std::cerr << " cumulative sum after word: " << cumulative_sum << std::endl;
        data[word_idx] |= set_bit;
        while (idx) {
            if ((idx & uint32_t(1)) == 0) {
                segment_tree[idx / 2]++;
            }
            idx /= 2;
        }
        return cumulative_sum;
    }
};