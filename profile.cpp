#include <array>
#include <iostream>
#include <random>

#include "avl_buf_sort.hpp"
#include "bf_buf_sort.hpp"
#include "block_buf_sort.hpp"
#include "bv_buf_sort.hpp"
#include "counters/counters.hpp"
#include "inversion_then_merge_sort.hpp"
#include "merge_inversions.hpp"
#include "rb_buf_sort.hpp"
#include "sb_merge_buf_sort.hpp"
#include "st_buf_sort.hpp"
#include "util.hpp"

const static uint32_t LIST_SIZE = uint32_t(1) << 14;

template <uint16_t buffer_size, uint32_t list_size>
class randomer {
  std::mt19937_64 gen;
  std::uniform_int_distribution<uint32_t> dist;

 public:
  randomer() : gen(), dist(0, list_size - buffer_size) {}

  void random_fill(B_type* arr) {
    for (size_t i = 0; i < buffer_size; ++i) {
      arr[i] = {dist(gen), bool(dist(gen) % 2)};
    }
  }
};

template <class sorter_t, uint16_t buffer_size, uint32_t list_size,
          class count_t>
void BM_buf_sort(count_t& counter) {
  std::array<B_type, buffer_size> buffer;
  randomer<buffer_size, list_size> rnd;
  counter.clear();
  for (uint16_t i = 0; i < 1000; ++i) {
    rnd.random_fill(buffer.data());
    counter.reset();
    sorter_t sorter;
    sorter.sort(buffer.data());
    counter.accumulate();
  }
  counter.output_counters(0, 1000);
}

#define MK_PROF(typ, bs)                      \
  std::cout << "Profiling: " << #typ << "\n"      \
            << "Block size: " << #bs << "\n"      \
            << "List size: " << LIST_SIZE << std::endl; \
  BM_buf_sort<typ<bs, LIST_SIZE>, bs, LIST_SIZE>(counter);

#define MK_RUN(typ)            \
  MK_PROF(typ, 8)   \
  MK_PROF(typ, 16)  \
  MK_PROF(typ, 32)  \
  MK_PROF(typ, 64)  \
  MK_PROF(typ, 128) \
  MK_PROF(typ, 256) \
  MK_PROF(typ, 512) \
  MK_PROF(typ, 1024)

int main() {
  count::Counters<false, 1, count::Counter::instructions,
                  count::Counter::branches, count::Counter::branch_miss,
                  count::Counter::L1D_miss, count::Counter::IPC>
      counter;
  MK_RUN(AVL_tree)
  MK_RUN(RB_tree)
  MK_RUN(S_tree)
  MK_RUN(BV_sorter)
  MK_RUN(BF_sorter)
  MK_RUN(Block_sort)
  MK_RUN(Merge_inversions)
  MK_RUN(SB_merge_inversions)
  MK_RUN(inversion_then_merge_sort)
  return 0;
}
