#include <benchmark/benchmark.h>

#include <array>
#include <random>

#include "avl_buf_sort.hpp"
#include "bf_buf_sort.hpp"
#include "block_buf_sort.hpp"
#include "bv_buf_sort.hpp"
#include "inversion_then_merge_sort.hpp"
#include "merge_inversions.hpp"
#include "rb_buf_sort.hpp"
#include "sb_merge_buf_sort.hpp"
#include "st_buf_sort.hpp"
#include "util.hpp"

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

template <class sorter_t, uint16_t buffer_size, size_t list_size>
void BM_buf_sort(benchmark::State& state) {
  std::array<B_type, buffer_size> buffer;
  randomer<buffer_size, list_size> rnd;
  for (auto _ : state) {
    state.PauseTiming();
    rnd.random_fill(buffer.data());
    state.ResumeTiming();
    sorter_t sorter;
    sorter.sort(buffer.data());
  }
}

#define MK_BM(typ) \
    BENCHMARK(BM_buf_sort<typ<8, 16384>, 8, 16384>); \
    BENCHMARK(BM_buf_sort<typ<16, 16384>, 16, 16384>); \
    BENCHMARK(BM_buf_sort<typ<32, 16384>, 32, 16384>); \
    BENCHMARK(BM_buf_sort<typ<64, 16384>, 64, 16384>); \
    BENCHMARK(BM_buf_sort<typ<128, 16384>, 128, 16384>); \
    BENCHMARK(BM_buf_sort<typ<256, 16384>, 256, 16384>); \
    BENCHMARK(BM_buf_sort<typ<512, 16384>, 512, 16384>); \
    BENCHMARK(BM_buf_sort<typ<1024, 16384>, 1024, 16384>);

MK_BM(AVL_tree)
MK_BM(RB_tree)
MK_BM(S_tree)
MK_BM(BV_sorter)
MK_BM(BF_sorter)
MK_BM(Block_sort)
MK_BM(Merge_inversions)
MK_BM(SB_merge_inversions)
MK_BM(inversion_then_merge_sort)

BENCHMARK_MAIN();