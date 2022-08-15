#include <iostream>
#include <vector>

struct AVL_node {
    uint32_t index;
    uint16_t buffer_index;
    uint16_t offset;
    uint16_t left_index;
    uint16_t right_index;
    uint16_t left_height;
    uint16_t right_height;
};

template <uint16_t size>
class AVL_tree {
  private:
    AVL_node nodes[size];
    std::pair<uint32_t, uint32_t> mapping[size];
    uint32_t root;
    uint32_t elems;

  public:
    AVL_tree(): root(0), elems(0) { }

    void insert(uint32_t i, uint16_t b_idx) {
        uint16_t loc = elems;
        nodes[elems++] = {i, b_idx, 0, 0, 0, 0, 0};
        if (elems == 1) [[unlikely]] {
            return;
        }
        increment(i, loc, root);
    }

    const std::pair<uint32_t, uint32_t>* const get_mapping() {
        walk(root, 0);
        return mapping;
    }

  private:
    bool increment(uint32_t v, uint16_t loc, uint16_t idx) {
        AVL_node* nd = nodes + idx;
        nd->index += nd->offset;
        nodes[nd->left_index].offset += nd->left_height ? nd->offset : 0;
        nodes[nd->right_index].offset += nd->right_height ? nd->offset : 0;
        nd->offset = 0;
        bool need_rotation = false;
        if (nd->index >= v) {
            nd->index++;
            nodes[nd->right_index].offset += 1;
            if (nd->left_height == 0) {
                nd->left_index = loc;
                nd->left_height = 1;
                return false;
            }
            if (increment(v, loc, nd->left_index)) {
                if (nodes[nd->left_index].left_height > nodes[nd->left_index].right_height) {
                    nd->left_index = rotate_right(nd->left_index);
                } else {
                    nd->left_index = rotate_left(nd->left_index);
                }
                nd->left_height = 1 + std::max(nodes[nd->left_index].left_height, nodes[nd->left_index].right_height);
            }
        } else {
            if (nd->right_height != 0) {
                nd->right_index = loc;
                nd->right_height = 1;
                return false;
            }
            if (increment(v, loc, nd->right_index)) {
                if (nodes[nd->right_index].left_height > nodes[nd->right_index].right_height) {
                    nd->right_index = rotate_right(nd->right_index);
                } else {
                    nd->right_index = rotate_left(nd->right_index);
                }
                nd->right_height = 1 + std::max(nodes[nd->right_index].left_height, nodes[nd->right_index].right_height);
            }
        }
        return std::abs(int(nd->left_height) - nd->right_height) > 1;
    }

    uint16_t walk(uint16_t idx, uint16_t insert_pos) {
        AVL_node* nd = nodes + idx;
        nd->index += nd->offset;
        nodes[nd->left_index].offset += nd->left_height ? nd->offset : 0;
        nodes[nd->right_index].offset += nd->right_height ? nd->offset : 0;
        if (nd->left_height) {
            insert_pos = walk(nd->left_index, insert_pos);
        }
        mapping[insert_pos++] = {nd->buffer_index, nd->index};
        if (nd->right_height) {
            insert_pos = walk(nd->right_index, insert_pos);
        }
        return insert_pos;
    }

    uint16_t rotate_right(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        uint16_t ret = nd->left_index;
        AVL_node* left_child = nodes + ret;
        if (left_child->right_height > left_child->left_height) {
            nd->left_index = rotate_sub_left(nd->left_index);
            left_child = nodes + nd->left_index;
        }
        nd->left_index = left_child->right_index;
        nd->left_height = left_child->right_height;
        left_child->right_index = idx;
        left_child->right_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_sub_left(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        uint16_t ret = nd->right_index;
        AVL_node* right_child = nodes + ret;
        nd->right_index = right_child->left_index;
        nd->right_height = right_child->left_height;
        right_child->left_index = idx;
        right_child->left_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_left(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        uint16_t ret = nd->right_index;
        AVL_node* right_child = nodes + ret;
        if (right_child->left_height > right_child->right_height) {
            nd->right_index = rotate_sub_right(nd->right_index);
            right_child = nodes + nd->right_index;
        }
        nd->right_index = right_child->left_index;
        nd->right_height = right_child->left_height;
        right_child->left_index = idx;
        right_child->left_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }

    uint16_t rotate_sub_right(uint16_t idx) {
        AVL_node* nd = nodes + idx;
        uint16_t ret = nd->left_index;
        AVL_node* left_child = nodes + ret;
        nd->left_index = left_child->right_index;
        nd->left_height = left_child->right_height;
        left_child->right_index = idx;
        left_child->right_height = 1 + std::max(nd->left_height, nd->right_height);
        return ret;
    }
};