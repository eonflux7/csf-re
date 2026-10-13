#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 32 bytes; isnil +45, recovered from binary.
using Node = legacy::vc71::tree_node<32>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0043e1f0(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, true>(self, node);
}

HOOK(0x0043E1F0, StlTree_0043e1f0, "stl");
