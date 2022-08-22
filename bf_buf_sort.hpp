#pragma once

#include <iostream>
#include <cstring>
#include <immintrin.h>
#include <bitset>

template<uint16_t buffer_size, uint32_t leaf_size>
class BF_sorter {
  private:
    inline static uint32_t items[buffer_size];
    uint16_t elems;

    class BF_ref {
      public:
        const uint32_t index;
        const uint16_t buffer_index;
        BF_ref(uint32_t i, uint16_t b_i) : index(i), buffer_index(b_i) {}
    };

    class BF_iterator {
      private:
        const uint32_t* a_ptr;
        uint16_t offset;
      public:
        BF_iterator(const uint32_t* ptr, uint16_t location) {
            a_ptr = ptr;
            offset = location;
        }

        bool operator==(const BF_iterator& rhs) const {
            return (a_ptr == rhs.a_ptr) && (offset == rhs.offset);
        }

        bool operator!=(const BF_iterator& rhs) const {
            return !operator==(rhs);
        }

        BF_ref operator*() const {
            return BF_ref(a_ptr[offset], offset);
        }

        BF_iterator & operator++() {
            offset++;
            return *this;
        }

        BF_iterator operator++(int) {
            BF_iterator clone(a_ptr, offset);
            offset++;
            return clone;
        }
    };

  public:
    BF_sorter() : elems(0) {}
    
    void insert(uint32_t v) {
        items[elems++] = v;
    }

    BF_iterator begin() {
        calculate_offsets();
        return BF_iterator(items, 0);
    }

    BF_iterator end() {
        return BF_iterator(items, elems);
    }

    void print(bool print_data = true) {
        for (uint16_t i = 0; i < elems; i++) {
            std::cerr << items[i] << ", " << i << std::endl;
        }
    }

  private:
    void calculate_offsets() {
        for (uint16_t i = 1; i < buffer_size; i++) {
            for (uint16_t j = 0; j < i; j++) {
                items[j] += uint16_t(items[j] <= items[i]);
            }
        }
    }
};