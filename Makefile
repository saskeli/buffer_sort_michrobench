.PHONY: clean all run

CFLAGS=-march=native -std=c++23 -Wall -Wextra -Wshadow -pedantic
BENCH=-isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
CMAKE_OPT=-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release

HEADERS = avl_buf_sort.hpp rb_buf_sort.hpp st_buf_sort.hpp bv_buf_sort.hpp \
          bf_buf_sort.hpp block_buf_sort.hpp merge_inversions.hpp util.hpp \
		  sb_merge_buf_sort.hpp inversion_then_merge_sort.hpp

MACHINE=$(shell lscpu | grep -o -P "(?<=Model name:).*" | sed -E 's/\s+//; s/\(\w+\)//g; s/\s/_/g')

all: bench profile

bench: bench.cpp $(HEADERS) benchmark/build/lib/libgtest.a
	g++ $(CFLAGS) -DNDEBUG -DDEPENDENCE_INSERTION -Ofast bench.cpp $(BENCH) -o bench

profile: profile.cpp $(HEADERS) counters/counters.hpp
	g++ $(CFLAGS) -DNDEBUG -Ofast profile.cpp -o profile

buf_sort: buf_sort.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o buf_sort buf_sort.cpp

benchmark/include:
	git submodule update --init

benchmark/build/lib/libgtest.a: | benchmark/include
	mkdir -p benchmark/build
	(cd benchmark; cmake $(CMAKE_OPT) -S . -B "build")
	(cd benchmark; cmake --build "build" --config Release)

counters/counters.hpp:
	git submodule update --init

clean:
	rm -f bench profile

run: all
	./bench | tee $(MACHINE).res
	./profile | tee $(MACHINE).prof
