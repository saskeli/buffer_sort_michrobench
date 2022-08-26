#pragma once

#include <iostream>
#include <immintrin.h>

#include "util.hpp"

template<uint16_t size, uint32_t n>
class RB_tree {
  private:
    static const constexpr uint16_t RED = uint16_t(1);
    static const constexpr uint16_t LEFT = uint16_t(1) << 1;
    static const constexpr uint16_t RIGHT = uint16_t(1) << 2;

    struct RB_node {
        uint32_t index;
        uint16_t offset;
        uint16_t payload;
        uint16_t left_index;
        uint16_t right_index;
        uint16_t meta;
    };

    inline static RB_node nodes[size];
    uint16_t root;
    uint16_t elems;

  public:
    RB_tree(): root(0), elems(0) {}

    void sort(B_type* buffer) {
        for (uint16_t i = 0; i < size; i++) {
            insert(buffer[i].first, buffer[i].second);
        }
        uint16_t i = 0;
        walk(root, i, buffer);
    }

    void insert(uint32_t i, u_int16_t v) {
        uint16_t loc = elems;
        if (elems == 0) [[unlikely]] {
            nodes[elems++] = {i, 0, v, 0, 0, 0};
            return;
        }
        nodes[elems++] = {i, 0, v, 0, 0, RED};
        int32_t res = increment(root, i, loc);
        if (res >= 0) {
            root = res;
        }
        if (nodes[root].meta & RED) {
            nodes[root].meta ^= RED;
        }
    }

    void print() {
        std::cerr << "{";
        print(root);
        std::cerr << "}" << std::endl;
    }

  private:
    int32_t increment(uint16_t index, uint32_t v, uint16_t location) {
        RB_node* nd = nodes + index;
        nd->index += nd->offset;
        nodes[nd->left_index].offset += nd->meta & LEFT ? nd->offset : 0;
        nodes[nd->right_index].offset += nd->meta & RIGHT ? nd->offset : 0;
        nd->offset = 0;
        if (nd->index >= v) {
            nd->index++;
            if (nd->meta & RIGHT) {
                nodes[nd->right_index].offset += 1;
            }
            if ((nd->meta & LEFT) == 0) [[unlikely]] {
                nd->left_index = location;
                nd->meta |= LEFT;
                return nd->meta & RED ? -1 : -3;
            }
            int32_t res = increment(nd->left_index, v, location);
            if (res == -3) [[likely]] {
                return -3;
            } else if (res == -2) {
                return nd->meta & RED ? -1 : -3;
            } else if (res == -1) [[unlikely]] {
                RB_node* lc = nodes + nd->left_index;
                if (nd->meta & RIGHT) {
                    RB_node* rc = nodes + nd->right_index;
                    if (rc->meta & RED) {
                        rc->meta ^= RED;
                        lc->meta ^= RED;
                        nd->meta |= RED;
                        return -2;
                    }
                }
                if (v >= lc->index) {
                    nd->left_index = rotate_left(nd->left_index);
                    lc = nodes + nd->left_index;
                }
                nd->meta |= RED;
                lc->meta ^= RED;
                return rotate_right(index);
            } else {
                nd->left_index = res;
                return -3;
            }
        } else {
            if ((nd->meta & RIGHT) == 0) [[unlikely]] {
                nd->right_index = location;
                nd->meta |= RIGHT;
                return nd->meta & RED ? -1 : -3;
            }
            int32_t res = increment(nd->right_index, v, location);
            if (res == -3) [[likely]] {
                return -3;
            } else if (res == -2) {
                return nd->meta & RED ? -1 : -3;
            } else if (res == -1) [[unlikely]] {
                RB_node* rc = nodes + nd->right_index;
                if (nd->meta & LEFT) {
                    RB_node* lc = nodes + nd->left_index;
                    if (lc->meta & RED) {
                        lc->meta ^= RED;
                        rc->meta ^= RED;
                        nd->meta |= RED;
                        return -2;
                    }
                }
                if (v < rc->index) {
                    nd->right_index = rotate_right(nd->right_index);
                    rc = nodes + nd->right_index;
                }
                nd->meta |= RED;
                rc->meta ^= RED;
                return rotate_left(index);
            } else {
                nd->right_index = res;
                return -3;
            }
        }
    }

    void walk(uint16_t idx, uint16_t &i, B_type* buffer) {
        RB_node* nd = nodes + idx;
        nd->index += nd->offset;
        if (nd->meta & LEFT) {
            nodes[nd->left_index].offset += nd->offset;
            walk(nd->left_index, i, buffer);
        }
        buffer[i].first = nd->index;
        buffer[i++].second = nd->payload;
        if (nd->meta & RIGHT) {
            nodes[nd->right_index].offset += nd->offset;
            walk(nd->right_index, i, buffer);
        }
    }

    uint16_t rotate_right(uint16_t idx) {
        RB_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->left_index;
        RB_node* left_child = nodes + ret;
        assert(left_child->offset == 0);
        nd->left_index = left_child->right_index;
        nd->meta ^= left_child->meta & RIGHT ? uint16_t(0) : LEFT;
        left_child->right_index = idx;
        left_child->meta |= RIGHT;
        return ret;
    }

    uint16_t rotate_left(uint16_t idx) {
        RB_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->right_index;
        RB_node* right_child = nodes + ret;
        assert(right_child->offset == 0);
        nd->right_index = right_child->left_index;
        nd->meta ^= nd->meta & LEFT ? uint16_t(0) : RIGHT;
        right_child->left_index = idx;
        right_child->meta |= LEFT;
        return ret;
    }

    void print(uint16_t index) {
        RB_node* nd = nodes + index;
        std::cerr << "\"index\": " << nd->index << ",\n";
        std::cerr << "\"buffer_index\": " << index << ",\n";
        std::cerr << "\"offset\": " << nd->offset << ",\n";
        std::cerr << "\"color\": " << (nd->meta & RED ? "\"RED\"" : "\"BLACK\"") << ",\n";
        std::cerr << "\"left_index\": " << nd->left_index << ",\n";
        std::cerr << "\"has_left\": " << bool(nd->meta & LEFT) << ",\n";
        std::cerr << "\"right_index\": " << nd->right_index << ",\n";
        std::cerr << "\"has_right\": " << bool(nd->meta & RIGHT) << ",\n";
        std::cerr << "\"children\": [";
        if (nd->meta & LEFT) {
            std::cerr << "{\n";
            print(nd->left_index);
            std::cerr << "}";
        } if (nd->meta & RIGHT) {
            if (nd->meta & LEFT) {
                std::cerr << ",";
            }
            std::cerr << "{\n";
            print(nd->right_index);
            std::cerr << "}";
        }
        std::cerr << "]\n";
    }
};