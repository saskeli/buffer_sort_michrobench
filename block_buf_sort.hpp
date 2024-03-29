#pragma once

#include <iostream>
#include <cstring>
#include <algorithm>

#include "util.hpp"
#include "bf_buf_sort.hpp"

template <uint16_t buffer_size, uint32_t n>
class Block_sort {
  private:
    const static constexpr uint16_t BLOCK_ELEMS = 32;
    const static constexpr uint16_t BLOCKS = buffer_size / BLOCK_ELEMS ? buffer_size / BLOCK_ELEMS : 1;

    inline static B_type scratch[buffer_size];
    inline static uint16_t block_indexes[BLOCKS];
    inline static uint16_t block_offsets[BLOCKS];

  public:

    void sort(B_type* buffer) {
        if constexpr (buffer_size < 32) {
            BF_sorter<buffer_size, n> bs;
            bs.sort(buffer);
            return;
        }
        for (uint16_t i = 1; i < BLOCK_ELEMS; i++) {
            for (uint16_t j = 0; j < i; j++) {
                buffer[j] += uint16_t(buffer[j] >= buffer[i]);
                if constexpr (BLOCKS > 1) {
                    buffer[j + 32] += uint16_t(buffer[j + 32] >= buffer[i + 32]);
                }
                if constexpr (BLOCKS > 2) {
                    buffer[j + 2 * 32] += uint16_t(buffer[j + 2 * 32] >= buffer[i + 2 * 32]);
                }
                if constexpr (BLOCKS > 3) {
                    buffer[j + 3 * 32] += uint16_t(buffer[j + 3 * 32] >= buffer[i + 3 * 32]);
                }
                if constexpr (BLOCKS > 4) {
                    buffer[j + 4 * 32] += uint16_t(buffer[j + 4 * 32] >= buffer[i + 4 * 32]);
                }
                if constexpr (BLOCKS > 5) {
                    buffer[j + 5 * 32] += uint16_t(buffer[j + 5 * 32] >= buffer[i + 5 * 32]);
                }
                if constexpr (BLOCKS > 6) {
                    buffer[j + 6 * 32] += uint16_t(buffer[j + 6 * 32] >= buffer[i + 6 * 32]);
                }
                if constexpr (BLOCKS > 7) {
                    buffer[j + 7 * 32] += uint16_t(buffer[j + 7 * 32] >= buffer[i + 7 * 32]);
                }
                if constexpr (BLOCKS > 8) {
                    buffer[j + 8 * 32] += uint16_t(buffer[j + 8 * 32] >= buffer[i + 8 * 32]);
                }
                if constexpr (BLOCKS > 9) {
                    buffer[j + 9 * 32] += uint16_t(buffer[j + 9 * 32] >= buffer[i + 9 * 32]);
                }
                if constexpr (BLOCKS > 10) {
                    buffer[j + 10 * 32] += uint16_t(buffer[j + 10 * 32] >= buffer[i + 10 * 32]);
                }
                if constexpr (BLOCKS > 11) {
                    buffer[j + 11 * 32] += uint16_t(buffer[j + 11 * 32] >= buffer[i + 11 * 32]);
                }
                if constexpr (BLOCKS > 12) {
                    buffer[j + 12 * 32] += uint16_t(buffer[j + 12 * 32] >= buffer[i + 12 * 32]);
                }
                if constexpr (BLOCKS > 13) {
                    buffer[j + 13 * 32] += uint16_t(buffer[j + 13 * 32] >= buffer[i + 13 * 32]);
                }
                if constexpr (BLOCKS > 14) {
                    buffer[j + 14 * 32] += uint16_t(buffer[j + 14 * 32] >= buffer[i + 14 * 32]);
                }
                if constexpr (BLOCKS > 15) {
                    buffer[j + 15 * 32] += uint16_t(buffer[j + 15 * 32] >= buffer[i + 15 * 32]);
                }
                if constexpr (BLOCKS > 16) {
                    buffer[j + 16 * 32] += uint16_t(buffer[j + 16 * 32] >= buffer[i + 16 * 32]);
                }
                if constexpr (BLOCKS > 17) {
                    buffer[j + 17 * 32] += uint16_t(buffer[j + 17 * 32] >= buffer[i + 17 * 32]);
                }
                if constexpr (BLOCKS > 18) {
                    buffer[j + 18 * 32] += uint16_t(buffer[j + 18 * 32] >= buffer[i + 18 * 32]);
                }
                if constexpr (BLOCKS > 19) {
                    buffer[j + 19 * 32] += uint16_t(buffer[j + 19 * 32] >= buffer[i + 19 * 32]);
                }
                if constexpr (BLOCKS > 20) {
                    buffer[j + 20 * 32] += uint16_t(buffer[j + 20 * 32] >= buffer[i + 20 * 32]);
                }
                if constexpr (BLOCKS > 21) {
                    buffer[j + 21 * 32] += uint16_t(buffer[j + 21 * 32] >= buffer[i + 21 * 32]);
                }
                if constexpr (BLOCKS > 22) {
                    buffer[j + 22 * 32] += uint16_t(buffer[j + 22 * 32] >= buffer[i + 22 * 32]);
                }
                if constexpr (BLOCKS > 23) {
                    buffer[j + 23 * 32] += uint16_t(buffer[j + 23 * 32] >= buffer[i + 23 * 32]);
                }
                if constexpr (BLOCKS > 24) {
                    buffer[j + 24 * 32] += uint16_t(buffer[j + 24 * 32] >= buffer[i + 24 * 32]);
                }
                if constexpr (BLOCKS > 25) {
                    buffer[j + 25 * 32] += uint16_t(buffer[j + 25 * 32] >= buffer[i + 25 * 32]);
                }
                if constexpr (BLOCKS > 26) {
                    buffer[j + 26 * 32] += uint16_t(buffer[j + 26 * 32] >= buffer[i + 26 * 32]);
                }
                if constexpr (BLOCKS > 27) {
                    buffer[j + 27 * 32] += uint16_t(buffer[j + 27 * 32] >= buffer[i + 27 * 32]);
                }
                if constexpr (BLOCKS > 28) {
                    buffer[j + 28 * 32] += uint16_t(buffer[j + 28 * 32] >= buffer[i + 28 * 32]);
                }
                if constexpr (BLOCKS > 29) {
                    buffer[j + 29 * 32] += uint16_t(buffer[j + 29 * 32] >= buffer[i + 29 * 32]);
                }
                if constexpr (BLOCKS > 30) {
                    buffer[j + 30 * 32] += uint16_t(buffer[j + 30 * 32] >= buffer[i + 30 * 32]);
                }
                if constexpr (BLOCKS > 31) {
                    buffer[j + 31 * 32] += uint16_t(buffer[j + 31 * 32] >= buffer[i + 31 * 32]);
                }
            }
        }
        for (uint16_t i = 0; i < BLOCKS; i++) {
            block_indexes[i] = i * BLOCK_ELEMS;
        }
        std::memset(block_offsets, 0, BLOCKS * sizeof(uint16_t));
        std::memcpy((void*)scratch, (void*)buffer, buffer_size * sizeof(B_type));
        for (uint16_t i = 0; i < BLOCKS; i++) {
            std::sort(scratch + i * BLOCK_ELEMS, scratch + (i + 1) * BLOCK_ELEMS);
        }
        //std::cerr << "after sorting scratch blocks" << std::endl;
        //print(buffer);
        for (uint16_t buffer_index = 0; buffer_index < buffer_size; buffer_index++) {
            uint16_t first_block = 0;
            while (block_indexes[first_block] >= (first_block + 1) * BLOCK_ELEMS) {
                first_block++;
            }
            uint16_t min_block = first_block++;
            uint32_t min_val = block_offsets[min_block] + scratch[block_indexes[min_block]];
            for (uint16_t i = first_block; i < BLOCKS; i++) {
                if (block_indexes[i] >= (i + 1) * BLOCK_ELEMS) {
                    continue;
                }
                uint32_t n_val = block_offsets[i] + scratch[block_indexes[i]];
                if (n_val <= min_val) {
                    min_block = i;
                    min_val = n_val;
                }
            }
            for (uint16_t i = 0; i < min_block; i++) {
                block_offsets[i]++;
            }
            //std::cerr << buffer_index <<  ": elem " << block_indexes[min_block] << " = " << scratch[block_indexes[min_block]] << " -> ";
            scratch[block_indexes[min_block]] += block_offsets[min_block];
            //std::cerr << scratch[block_indexes[min_block]] << std::endl;
            buffer[buffer_index] = scratch[block_indexes[min_block]++];
        }
    }

    void print(B_type* buffer) {
        std::cerr << "Buffer:" << std::endl;
        for (uint16_t i = 0; i < BLOCKS; i++) {
            std::cerr << "Block " << i << std::endl;
            for (uint16_t j = 0; j < BLOCK_ELEMS / 4; j++) {
                for (uint16_t k = 0; k < 4; k++) {
                    B_type* el = buffer + i * BLOCK_ELEMS + 4 * j + k;
                    std::cerr << el << "\t";
                }
                std::cerr << std::endl;
            }
        }

        std::cerr << "Block index:" << std::endl;
        for (uint16_t i = 0; i < BLOCKS; i++) {
            std::cerr << block_indexes[i] << std::endl;
        }

        std::cerr << "Block offset:" << std::endl;
        for (uint16_t i = 0; i < BLOCKS; i++) {
            std::cerr << block_offsets[i] << std::endl;
        }

        std::cerr << "Scratch:" << std::endl;
        for (uint16_t i = 0; i < BLOCKS; i++) {
            std::cerr << "Block " << i << std::endl;
            for (uint16_t j = 0; j < BLOCK_ELEMS / 4; j++) {
                for (uint16_t k = 0; k < 4; k++) {
                    B_type* el = scratch + i * BLOCK_ELEMS + 4 * j + k;
                    std::cerr << el << "\t";
                }
                std::cerr << std::endl;
            }
        }
    }
};