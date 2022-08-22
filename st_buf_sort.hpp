#pragma once

#include <iostream>
#include <immintrin.h>
#include <cstring>
#include <bitset>

template<uint16_t size, uint32_t n>
class S_tree {
  private:
    static const constexpr uint64_t WORD_BITS = 64;
    static const constexpr uint64_t WORDS = n / WORD_BITS;
    inline static uint32_t items[size];
    inline static uint64_t data[WORDS];
    inline static uint16_t segment_tree[WORDS];

    static_assert(__builtin_popcountll(n) == 1);
    static_assert(n > 64);

    uint16_t elems;

    class ST_ref {
      public:
        const uint32_t index;
        const uint16_t buffer_index;
        ST_ref(uint32_t i, uint16_t b_i) : index(i), buffer_index(b_i) {}
    };

    class ST_iterator {
      private:
        const uint32_t* a_ptr;
        uint16_t offset;
      public:
        ST_iterator(const uint32_t* ptr, uint16_t location) {
            a_ptr = ptr;
            offset = location;
        }

        bool operator==(const ST_iterator& rhs) const {
            return (a_ptr == rhs.a_ptr) && (offset == rhs.offset);
        }

        bool operator!=(const ST_iterator& rhs) const {
            return !operator==(rhs);
        }

        ST_ref operator*() const {
            return ST_ref(a_ptr[offset], offset);
        }

        ST_iterator & operator++() {
            offset++;
            return *this;
        }

        ST_iterator operator++(int) {
            ST_iterator clone(a_ptr, offset);
            offset++;
            return clone;
        }
    };
  public:
    S_tree() : elems(0) {}

    void insert(uint32_t v) {
        items[elems++] = v;
    }

    ST_iterator begin() {
        calculate_offsets();
        return ST_iterator(items, 0);
    }

    ST_iterator end() {
        return ST_iterator(items, elems);
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
        for (uint16_t i = 0; i < elems; i++) {
            std::cout << items[i] << " " << i << std::endl;
        }
    }

  private:

    void calculate_offsets() {
        memset(data, 0, WORDS * sizeof(uint64_t));
        memset(segment_tree, 0, WORDS * sizeof(uint16_t));
        for (uint16_t i = elems - 1; i < elems; i--) {
            //std::cerr << "updating " << items[i] << " at " << i << std::endl;
            items[i] += update(items[i]);
            //print();
        }
    }

    uint32_t update(uint32_t index) {
        //std::cerr << "update(" << index << ")" << std::endl;
        uint32_t idx = 1;
        uint32_t block_elems = n / 2;
        uint32_t cumulative_sum = 0;
        while (idx < WORDS) {
            uint32_t block_zeros = block_elems - segment_tree[idx];
            //std::cerr << " Looking at " << idx << " with " << block_zeros << " zeros to the left" << std::endl;
            if (block_zeros < index) {
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
        uint16_t word_offset = 63 - __builtin_clzll(_pdep_u64(uint64_t(1) << index, ~data[word_idx]));
        uint64_t set_bit = uint64_t(1) << word_offset;
        cumulative_sum += __builtin_popcountll(data[word_idx] & (set_bit - 1));
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