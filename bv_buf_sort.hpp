#pragma once

#include <algorithm>
#include <iostream>
#include <cstring>
#include <immintrin.h>
#include <bitset>

#include "util.hpp"

template<uint16_t buffer_size, uint32_t leaf_size>
class BV_sorter {
  private:
    static const constexpr uint32_t WORD_BITS = 64;
    static const constexpr uint32_t SMALL_BLOCK_SIZE = 512;
    static const constexpr uint32_t SMALL_BLOCK_WORDS = SMALL_BLOCK_SIZE / WORD_BITS;
    static const constexpr uint32_t BIG_BLOCK_SIZE = 8 * SMALL_BLOCK_SIZE;
    static const constexpr uint32_t BIG_BLOCK_COUNT = 4;
    static const constexpr uint32_t BIG_BLOCK_WORDS = BIG_BLOCK_SIZE / WORD_BITS;
    static const constexpr uint32_t SMALL_BLOCK_COUNT = 8;
    static const constexpr uint32_t WORDS = leaf_size / WORD_BITS;
    static const constexpr uint32_t B_TREE_SIZE = BIG_BLOCK_COUNT + BIG_BLOCK_COUNT * SMALL_BLOCK_COUNT;

    inline static uint64_t data[WORDS];
    inline static uint16_t b_tree[B_TREE_SIZE];

    static_assert(leaf_size == 16384);

  public:
    
    void sort(B_type* buffer) {
        memset(data, 0, WORDS * sizeof(uint64_t));
        memset(b_tree, 0, B_TREE_SIZE * sizeof(uint16_t));
        for (uint16_t i = buffer_size - 1; i < buffer_size; i--) {
            //std::cerr << "updating " << items[i] << " at " << i << std::endl;
            buffer[i].first += update(buffer[i].first);
            //print();
        }
        std::sort(buffer, buffer + buffer_size);
    }

    void print(bool print_data = true) {
        uint32_t block_index = 0;
        for (uint16_t i = 0; i < BIG_BLOCK_COUNT; i++) {
            std::cerr << b_tree[block_index++] << " ";
        }
        std::cerr << std::endl;
        for (uint16_t i = 0; i < BIG_BLOCK_COUNT; i++) {
            for (uint16_t j = 0; j < SMALL_BLOCK_COUNT; j++) {
                std::cerr << b_tree[block_index++] << " ";
            }
            std::cerr << BIG_BLOCK_SIZE * (i + 1) << std::endl;
        }
        if (print_data) {
            for (uint32_t i = 0; i < WORDS; i++) {
                std::cerr << std::bitset<WORD_BITS>(data[i]) << " " << (i * WORD_BITS) << std::endl;
            }
        }
        
    }

  private:
    
    uint16_t update(uint32_t v) {
        //std::cerr << "Before update:" << std::endl;
        //print(false);
        // Big blocks
        uint16_t cum_sum = 0;
        uint32_t bb_cum = 0;
        uint16_t bb_i = 0;
        uint32_t bb_offset = 0;
        for (uint16_t i = 0; i < BIG_BLOCK_COUNT; i++) {
            uint32_t b_zeros = BIG_BLOCK_SIZE - b_tree[i];
            bb_cum += b_zeros;
            bool r = bb_cum <= v;
            bb_i += r * 1;
            bb_offset += r * b_zeros; 
            cum_sum += r * b_tree[i];
        }
        b_tree[bb_i]++;
        //std::cerr << "Cumulative sum before target: " << cum_sum << " for bb_i = " << bb_i << std::endl;
        //std::cerr << "After bb:" << std::endl;
        //print(false);

        // Small blocks
        v -= bb_offset;
        uint32_t sb_cum = 0;
        uint16_t sb_i = 0;
        uint32_t sb_offset = 0;
        uint32_t block_offset = BIG_BLOCK_COUNT + bb_i * SMALL_BLOCK_COUNT;
        for (uint16_t i = 0; i < SMALL_BLOCK_COUNT; i++) {
            uint32_t b_zeros = SMALL_BLOCK_SIZE - b_tree[block_offset + i]; 
            sb_cum += b_zeros;
            bool r = sb_cum <= v;
            sb_i += r * 1;
            sb_offset += r * b_zeros; 
            cum_sum += r * b_tree[block_offset + i];
        }
        b_tree[BIG_BLOCK_COUNT + bb_i * SMALL_BLOCK_COUNT + sb_i]++;
        //std::cerr << "Cumulative sum before target: " << cum_sum << " for sb_i = " << sb_i << std::endl;
        //std::cerr << "After sb:" << std::endl;
        //print(false);

        // Words
        v -= sb_offset;
        uint32_t bv_cum = 0;
        uint16_t bv_i = 0;
        uint32_t bv_offset = 0;
        block_offset = bb_i * BIG_BLOCK_WORDS + sb_i * SMALL_BLOCK_WORDS;
        //std::cerr << "word index = " << block_offset << std::endl;
        for (uint16_t i = 0; i < SMALL_BLOCK_WORDS; i++) {
            uint32_t b_zeros = __builtin_popcountll(~data[block_offset + i]); 
            bv_cum += b_zeros;
            bool r = bv_cum <= v;
            bv_i += r * 1;
            bv_offset += r * b_zeros; 
            cum_sum += r * __builtin_popcountll(data[block_offset + i]);
        }
        //std::cerr << "Cumulative sum before target: " << cum_sum << " for word = " << bv_i + block_offset << std::endl;
        //std::cerr << "After words:" << std::endl;
        //print(false);

        // Single word
        v -= bv_offset;
        uint32_t w_i = block_offset + bv_i;
        uint16_t word_offset = 63 - __builtin_clzll(_pdep_u64(uint64_t(1) << v, ~data[w_i]));
        uint64_t set_bit = uint64_t(1) << word_offset;
        cum_sum += __builtin_popcountll(data[w_i] & (set_bit - 1));
        data[w_i] |= set_bit;
        //std::cerr << "Cumulative sum before target: " << cum_sum << std::endl;
        return cum_sum;
    }
};