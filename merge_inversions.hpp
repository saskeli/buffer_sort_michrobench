#pragma once

#include <iostream>
#include <immintrin.h>

#include "util.hpp"

template <uint16_t buffer_size, uint32_t n>
class Merge_inversions {
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
        //print(buffer);
        sort<buffer_size>(scratch, buffer);
    }

  private:
    template<uint16_t block_size>
    void sort(B_type* source, B_type* target) {
        if constexpr (block_size == 2) {
            twosort(target);
            return;
        } else if constexpr (block_size == 4 && __builtin_clz(buffer_size) & uint16_t(1)) {
            foursort(target);
            return;
        } else {
            const constexpr uint16_t sub_block_size = block_size / 2;
            sort<sub_block_size>(target, source);
            for (uint16_t offset = 0; offset < buffer_size; offset += block_size) {
                uint16_t inversions = 0;
                uint16_t a_idx = offset;
                uint16_t b_idx = offset + sub_block_size;
                uint16_t a_limit = b_idx;
                uint16_t b_limit = offset + block_size;
                for (uint16_t i = 0; i < block_size; i++) {
                    if (a_idx < a_limit) [[likely]] {
                        if (b_idx < b_limit) [[likely]] {
                            if (source[a_idx].first + inversions < source[b_idx].first) {
                                target[offset + i] = source[a_idx++];
                                target[offset + i].first += inversions;
                            } else {
                                target[offset + i] = source[b_idx++];
                                inversions++;
                            }
                        } else {
                            target[offset + i] = source[a_idx++];
                            target[offset + i].first += inversions;
                        }
                    } else {
                        target[offset + i] = source[b_idx++];
                    }
                }
            }
            //std::cerr << "Merge to " << block_size << ": " << std::endl;
            //print(buf);
        }
    }

    void twosort(B_type* source) {
        for (uint16_t i = 0; i < buffer_size; i += 2) {
            if (source[i + 1].first <= source[i].first) {
                source[i].first++;
                std::swap(source[i], source[i + 1]);
            }
        }
        //std::cerr << "two sort:" << std::endl;
        //print(buf);
    }

    void foursort(B_type* source) {
        for (uint16_t i = 0; i < buffer_size; i += 4) {
            source[i].first += uint32_t(source[i + 1].first <= source[i].first);

            source[i + 1].first += uint32_t(source[i + 2].first <= source[i + 1].first);
            source[i].first += uint32_t(source[i + 2].first <= source[i].first);

            source[i + 2].first += uint32_t(source[i + 3].first <= source[i + 2].first);
            source[i + 1].first += uint32_t(source[i + 3].first <= source[i + 1].first);
            source[i].first += uint32_t(source[i + 3].first <= source[i].first);
            if (source[i + 2].first > source[i + 3].first) {
                std::swap(source[i + 2], source[i + 3]);
            } 
            if (source[i].first > source[i + 1].first) {
                std::swap(source[i], source[i + 1]);
            }
            if (source[i].first > source[i + 2].first) {
                std::swap(source[i], source[i + 2]);
            }
            if (source[i + 1].first > source[i + 3].first) {
                std::swap(source[i + 1], source[i + 3]);
            }
            if (source[i + 1].first > source[i + 2].first) {
                std::swap(source[i + 1], source[i + 2]);
            }
        }
        //std::cerr << "Four sort:" << std::endl;
        //print(buf);
    }

    void print(B_type* buffer) {
        std::cerr << "buffer:" << std::endl;
        for (uint16_t i = 0; i < buffer_size; i++) {
            std::cerr << "(" << buffer[i].first << ", " << buffer[i].second << ") ";
            if (i % 4 == 3) {
                std::cerr << std::endl;
            }
        }
        std::cerr << "scratch:" << std::endl;
        for (uint16_t i = 0; i < buffer_size; i++) {
            std::cerr << "(" << scratch[i].first << ", " << scratch[i].second << ") ";
            if (i % 4 == 3) {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }
};