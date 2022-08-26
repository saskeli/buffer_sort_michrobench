#pragma once

#include <iostream>
#include <queue>
#include <cassert>
#include <iterator>

#include "util.hpp"


template <uint16_t size, uint32_t n>
class AVL_tree {
  private:
    struct AVL_node {
        uint32_t index;
        uint16_t offset;
        uint16_t payload;
        uint16_t left_index;
        uint16_t right_index;
        uint16_t left_height;
        uint16_t right_height;
    };

    inline static AVL_node nodes[size];
    uint16_t root;
    uint16_t elems;

  public:
    AVL_tree(): root(0), elems(0) { }

    void sort(B_type* buffer) {
        for (uint16_t i = 0; i < size; i++) {
            insert(buffer[i].first, buffer[i].second);
        }
        uint16_t i = 0;
        walk(root, i, buffer);
    }

    void insert(uint32_t i, uint16_t v) {
        uint16_t loc = elems;
        nodes[elems++] = {i, 0, v, 0, 0, 0, 0};
        if (elems == 1) [[unlikely]] {
            return;
        }
        if (increment(i, loc, root)) [[unlikely]] {
            if (nodes[root].left_height > nodes[root].right_height) {
                root = rotate_right(root);
            } else {
                root = rotate_left(root);
            }
        }
    }

    void print() {
        std::cerr << "{";
        print(root);
        std::cerr << "}" << std::endl;
    }

  private:
    bool increment(uint32_t v, uint16_t loc, uint16_t idx) {
        AVL_node* nd = nodes + idx;
        nd->index += nd->offset;
        nodes[nd->left_index].offset += nd->left_height ? nd->offset : 0;
        nodes[nd->right_index].offset += nd->right_height ? nd->offset : 0;
        nd->offset = 0;
        if (nd->index >= v) {
            nd->index++;
            if (nd->right_height) {
                nodes[nd->right_index].offset += 1;
            }
            if (nd->left_height == 0) [[unlikely]] {
                nd->left_index = loc;
                nd->left_height = 1;
                return false;
            }
            if (increment(v, loc, nd->left_index)) [[unlikely]] {
                if (nodes[nd->left_index].left_height > nodes[nd->left_index].right_height) {
                    nd->left_index = rotate_right(nd->left_index);
                } else {
                    nd->left_index = rotate_left(nd->left_index);
                }
            }
            nd->left_height = 1 + std::max(nodes[nd->left_index].left_height, nodes[nd->left_index].right_height);
        } else {
            if (nd->right_height == 0) [[unlikely]] {
                nd->right_index = loc;
                nd->right_height = 1;
                return false;
            }
            if (increment(v, loc, nd->right_index)) [[unlikely]] {
                if (nodes[nd->right_index].left_height > nodes[nd->right_index].right_height) {
                    nd->right_index = rotate_right(nd->right_index);
                } else {
                    nd->right_index = rotate_left(nd->right_index);
                }
            }
            nd->right_height = 1 + std::max(nodes[nd->right_index].left_height, nodes[nd->right_index].right_height);
        }
        return std::abs(int(nd->left_height) - nd->right_height) > 1;
    }

    void walk(uint16_t idx, uint16_t &i, B_type* buffer) {
        AVL_node* nd = nodes + idx;
        nd->index += nd->offset;
        nodes[nd->left_index].offset += nd->left_height ? nd->offset : 0;
        nodes[nd->right_index].offset += nd->right_height ? nd->offset : 0;
        if (nd->left_height) {
            walk(nd->left_index, i, buffer);
        }
        buffer[i].first = nd->index;
        buffer[i++].second = nd->payload;
        if (nd->right_height) {
            walk(nd->right_index, i, buffer);
        }
    }

    uint16_t rotate_right(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->left_index;
        AVL_node* left_child = nodes + ret;
        assert(left_child->offset == 0);
        if (left_child->right_height > left_child->left_height) {
            nd->left_index = rotate_sub_left(nd->left_index);
            ret = nd->left_index;
            left_child = nodes + ret;
            assert(left_child->offset == 0);
        }
        nd->left_index = left_child->right_index;
        nd->left_height = left_child->right_height;
        left_child->right_index = idx;
        left_child->right_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_sub_left(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->right_index;
        AVL_node* right_child = nodes + ret;
        assert(right_child->offset == 0);
        nd->right_index = right_child->left_index;
        nd->right_height = right_child->left_height;
        right_child->left_index = idx;
        right_child->left_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_left(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->right_index;
        AVL_node* right_child = nodes + ret;
        assert(right_child->offset == 0);
        if (right_child->left_height > right_child->right_height) {
            nd->right_index = rotate_sub_right(nd->right_index);
            ret = nd->right_index;
            right_child = nodes + ret;
            assert(right_child->offset == 0);
        }
        nd->right_index = right_child->left_index;
        nd->right_height = right_child->left_height;
        right_child->left_index = idx;
        right_child->left_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_sub_right(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        assert(nd->offset == 0);
        uint16_t ret = nd->left_index;
        AVL_node* left_child = nodes + ret;
        assert(left_child->offset == 0);
        nd->left_index = left_child->right_index;
        nd->left_height = left_child->right_height;
        left_child->right_index = idx;
        left_child->right_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    void print(uint16_t index) {
        AVL_node* nd = nodes + index;
        std::cerr << "\"index\": " << nd->index << ",\n";
        std::cerr << "\"buffer_index\": " << index << ",\n";
        std::cerr << "\"offset\": " << nd->offset << ",\n";
        std::cerr << "\"left_index\": " << nd->left_index << ",\n";
        std::cerr << "\"left_height\": " << nd->left_height << ",\n";
        std::cerr << "\"right_index\": " << nd->right_index << ",\n";
        std::cerr << "\"right_height\": " << nd->right_height << ",\n";
        std::cerr << "\"children\": [";
        if (nd->left_height) {
            std::cerr << "{\n";
            print(nd->left_index);
            std::cerr << "}";
        } if (nd->right_height) {
            if (nd->left_height) {
                std::cerr << ",";
            }
            std::cerr << "{\n";
            print(nd->right_index);
            std::cerr << "}";
        }
        std::cerr << "]\n";
    }
};