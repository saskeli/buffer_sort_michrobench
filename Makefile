CFLAGS = -std=c++2a -Wall -Wextra -Wshadow -pedantic -march=native

HEADERS = avl_buf_sort.hpp

.PHONY: debug

all: buf_sort

buf_sort: buf_sort.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o buf_sort buf_sort.cpp

debug: buf_sort.cpp $(HEADERS)
	g++ $(CFLAGS) -DDEBUG -g -o buf_sort buf_sort.cpp

%.hpp: