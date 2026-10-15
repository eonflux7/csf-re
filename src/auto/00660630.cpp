#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"
using Node = legacy::vc71::tree_node<8>;
static Node* __fastcall SharedHelper(Node** self, void*) {
    Node* node = *self;
    if (node->isnil) { *self = node->right; return *self; }
    Node* next = node->left;
    if (!next->isnil) {
        while (!next->right->isnil) next = next->right;
        *self = next; return next->right;
    }
    Node* parent = node->parent;
    while (!parent->isnil && *self == parent->left) {
        *self = parent; parent = parent->parent;
    }
    if (!parent->isnil) *self = parent;
    return parent;
}
HOOK(0x00660630, SharedHelper, "stl");
