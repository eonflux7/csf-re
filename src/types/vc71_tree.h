// Binary-derived VC7.1 tree helpers. No library implementation source used.
#pragma once
#include <cstddef>
#include <cstdint>
#include "globals.h"

namespace legacy { namespace vc71 {
template <size_t ValueBytes> struct tree_node {
    tree_node* left;
    tree_node* parent;
    tree_node* right;
    uint8_t value[ValueBytes];
    uint8_t color;
    uint8_t isnil;
};
template <class Node> struct tree {
    uint32_t allocator_comparator;
    Node* head;
    uint32_t size;
};
static_assert(sizeof(void*) == 4);
static_assert(offsetof(tree_node<4>, color) == 16);
static_assert(offsetof(tree_node<8>, color) == 20);
static_assert(offsetof(tree_node<12>, color) == 24);
static_assert(offsetof(tree_node<32>, isnil) == 45);
static_assert(offsetof(tree_node<36>, color) == 48);
static_assert(offsetof(tree_node<60>, color) == 72);
using tree_layout = tree<tree_node<4>>;
static_assert(sizeof(tree_layout) == 12);
static_assert(offsetof(tree_layout, head) == 4);
static_assert(offsetof(tree_layout, size) == 8);

CSF_FUNC(void*, __fastcall, tree_string_assign, 0x00403C60, (void* self, void* edx, const char* text, uint32_t count));
CSF_FUNC(void*, __fastcall, tree_string_copy, 0x00403B60, (void* self, void* edx, void* source, uint32_t position, uint32_t count));
CSF_FUNC(void*, __fastcall, tree_exception_construct, 0x007AF6A0, (void* self, void* edx));
// The throw boundary is nonreturning in the game. Retain EAX if an offline
// boundary mock returns: erase's original falls through with that EAX as node.
CSF_FUNC(uint32_t, __stdcall, tree_throw, 0x007AF666, (void* object, void* info));

struct tree_string { uint32_t allocator; uint8_t buffer[16]; uint32_t length; uint32_t capacity; };
struct tree_exception { void** vtable; uint8_t base[8]; tree_string message; };
template <bool Length> __forceinline uint32_t tree_error(void* edx) {
    tree_string message;
    message.capacity = 15;
    message.length = 0;
    message.buffer[0] = 0;
    if constexpr (Length) tree_string_assign(&message, edx, CSF_STR(0x00830FF4, "map/set<T> too long"), 19);
    else tree_string_assign(&message, edx, CSF_STR(0x00830FC4, "invalid map/set<T> iterator"), 27);
    tree_exception exception;
    tree_exception_construct(&exception, nullptr);
    exception.vtable = CSF_VTBL(0x008769D4);
    exception.message.capacity = 15;
    exception.message.length = 0;
    exception.message.buffer[0] = 0;
    tree_string_copy(&exception.message, nullptr, &message, 0, 0xFFFFFFFFu);
    if constexpr (Length) {
        exception.vtable = CSF_VTBL(0x008769E0);
        return tree_throw(&exception, reinterpret_cast<void*>(CSF_ADDR_DATA(0x008867F0)));
    } else {
        exception.vtable = CSF_VTBL(0x008769EC);
        return tree_throw(&exception, reinterpret_cast<void*>(CSF_ADDR_DATA(0x00887184)));
    }
}

template <class Node, bool Maximum> __forceinline Node* tree_extreme(Node* node) {
    Node* next = Maximum ? node->right : node->left;
    while (!next->isnil) {
        node = next;
        next = Maximum ? node->right : node->left;
    }
    return node;
}

template <class Node, bool Left> __forceinline Node* tree_rotate(tree<Node>* self, Node* node) {
    Node* pivot = Left ? node->right : node->left;
    if constexpr (Left) node->right = pivot->left;
    else node->left = pivot->right;
    Node* moved = Left ? pivot->left : pivot->right;
    if (!moved->isnil) moved->parent = node;
    pivot->parent = node->parent;
    if (node == self->head->parent) self->head->parent = pivot;
    else if (node == (Left ? node->parent->left : node->parent->right)) {
        if constexpr (Left) node->parent->left = pivot;
        else node->parent->right = pivot;
    } else {
        if constexpr (Left) node->parent->right = pivot;
        else node->parent->left = pivot;
    }
    if constexpr (Left) pivot->left = node;
    else pivot->right = node;
    node->parent = pivot;
    return pivot;
}

template <class Ops> __forceinline typename Ops::Node** tree_insert(
    tree<typename Ops::Node>* self, void* edx, typename Ops::Node** result,
    uint8_t left, typename Ops::Node* parent, const void* value) {
    using Node = typename Ops::Node;
    if (self->size >= 0xFFFFFFFEu) tree_error<true>(edx);
    Node* added = Ops::buy(self->head, parent, self->head, value, 0);
    ++self->size;
    if (parent == self->head) {
        self->head->parent = added;
        self->head->left = added;
        self->head->right = added;
    } else if (left) {
        parent->left = added;
        if (parent == self->head->left) self->head->left = added;
    } else {
        parent->right = added;
        if (parent == self->head->right) self->head->right = added;
    }
    Node* node = added;
    while (!node->parent->color) {
        Node* p = node->parent;
        Node* grand = p->parent;
        Node* uncle = grand->left;
        if (p == grand->left) {
            uncle = grand->right;
            if (uncle->color) {
                if (node == p->right) { node = p; Ops::lrotate(self, node); }
                node->parent->color = 1;
                node->parent->parent->color = 0;
                Ops::rrotate(self, node->parent->parent);
            } else {
                p->color = 1; uncle->color = 1; grand->color = 0; node = grand;
            }
        } else if (uncle->color) {
            if (node == p->left) { node = p; Ops::rrotate(self, node); }
            node->parent->color = 1;
            node->parent->parent->color = 0;
            Ops::lrotate(self, node->parent->parent);
        } else {
            p->color = 1; uncle->color = 1; grand->color = 0; node = grand;
        }
    }
    self->head->parent->color = 1;
    *result = added;
    return result;
}

// Draft family: no erase hooks are emitted in this slice. Stock emudiff's
// RaiseException cleanup blocks all six candidates (see the worker report).
template <class Ops> __forceinline typename Ops::Node** tree_erase(
    tree<typename Ops::Node>* self, void* edx, typename Ops::Node** result,
    typename Ops::Node* position) {
    using Node = typename Ops::Node;
    Node* removed = position;
    if (position->isnil) removed = reinterpret_cast<Node*>(tree_error<false>(edx));
    Ops::advance(&position);
    Node* replacement;
    Node* parent;
    Node* successor = removed;
    if (removed->left->isnil) replacement = removed->right;
    else if (removed->right->isnil) replacement = removed->left;
    else { successor = position; replacement = successor->right; }
    if (successor == removed) {
        parent = removed->parent;
        if (!replacement->isnil) replacement->parent = parent;
        if (self->head->parent == removed) self->head->parent = replacement;
        else if (parent->left == removed) parent->left = replacement;
        else parent->right = replacement;
        if (self->head->left == removed)
            self->head->left = replacement->isnil ? parent : Ops::min(replacement);
        if (self->head->right == removed) {
            Node* head = self->head;
            head->right = replacement->isnil ? parent : Ops::max(replacement);
        }
    } else {
        removed->left->parent = successor;
        successor->left = removed->left;
        if (successor == removed->right) parent = successor;
        else {
            parent = successor->parent;
            if (!replacement->isnil) replacement->parent = parent;
            parent->left = replacement;
            successor->right = removed->right;
            removed->right->parent = successor;
        }
        if (self->head->parent == removed) self->head->parent = successor;
        else if (removed->parent->left == removed) removed->parent->left = successor;
        else removed->parent->right = successor;
        successor->parent = removed->parent;
        uint8_t color = successor->color;
        successor->color = removed->color;
        removed->color = color;
    }
    if (removed->color == 1) {
        while (replacement != self->head->parent && replacement->color == 1) {
            Node* sibling = parent->left;
            if (replacement == parent->left) {
                sibling = parent->right;
                if (!sibling->color) {
                    sibling->color = 1; parent->color = 0;
                    Ops::lrotate(self, parent);
                    sibling = parent->right;
                }
                if (!sibling->isnil) {
                    if (sibling->left->color != 1 || sibling->right->color != 1) {
                        if (sibling->right->color == 1) {
                            sibling->left->color = 1; sibling->color = 0;
                            Ops::rrotate(self, sibling);
                            sibling = parent->right;
                        }
                        sibling->color = parent->color;
                        parent->color = 1;
                        sibling->right->color = 1;
                        Ops::lrotate(self, parent);
                        break;
                    }
                    sibling->color = 0;
                }
            } else {
                if (!sibling->color) {
                    sibling->color = 1; parent->color = 0;
                    Ops::rrotate(self, parent);
                    sibling = parent->left;
                }
                if (!sibling->isnil) {
                    if (sibling->right->color != 1 || sibling->left->color != 1) {
                        if (sibling->left->color == 1) {
                            sibling->right->color = 1; sibling->color = 0;
                            Ops::lrotate(self, sibling);
                            sibling = parent->left;
                        }
                        sibling->color = parent->color;
                        parent->color = 1;
                        sibling->left->color = 1;
                        Ops::rrotate(self, parent);
                        break;
                    }
                    sibling->color = 0;
                }
            }
            replacement = parent;
            bool root = parent == self->head->parent;
            parent = parent->parent;
            if (root) break;
        }
        replacement->color = 1;
    }
    Ops::destroy(removed);
    if (self->size) --self->size;
    *result = position;
    return result;
}
} }
