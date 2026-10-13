#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 8 bytes; isnil +21, recovered from binary.
using Node = legacy::vc71::tree_node<8>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0069a6b0(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, false>(self, node);
}

HOOK(0x0069A6B0, StlTree_0069a6b0, "stl");
