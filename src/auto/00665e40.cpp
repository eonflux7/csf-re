#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"
using Node = legacy::vc71::tree_node<8>;
static Node* __fastcall SharedHelper(Node** self, void*) {
    Node* node = *self;
    if (node->isnil) return node;
    Node* next = node->right;
    if (!next->isnil) {
        while (!next->left->isnil) next = next->left;
        *self = next; return next->left;
    }
    Node* parent = node->parent;
    while (!parent->isnil && *self == parent->right) {
        *self = parent; parent = parent->parent;
    }
    *self = parent; return parent;
}
HOOK(0x00665E40, SharedHelper, "stl");
