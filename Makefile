CFLAGS = -std=c++2a -Wall -Wextra -Wshadow -pedantic -march=native

HEADERS = avl_buf_sort.hpp rb_buf_sort.hpp st_buf_sort.hpp bv_buf_sort.hpp \
          bf_buf_sort.hpp block_buf_sort.hpp merge_inversions.hpp util.hpp \
		  sb_merge_buf_sort.hpp inversion_then_merge_sort.hpp

.PHONY: debug

all: buf_sort

buf_sort: buf_sort.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o buf_sort buf_sort.cpp

debug: buf_sort.cpp $(HEADERS)
	g++ $(CFLAGS) -DDEBUG -O0 -g -o buf_sort buf_sort.cpp

%.hpp: