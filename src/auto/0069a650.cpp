#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 8 bytes; isnil +21, recovered from binary.
using Node = legacy::vc71::tree_node<8>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0069a650(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, true>(self, node);
}

HOOK(0x0069A650, StlTree_0069a650, "stl");
