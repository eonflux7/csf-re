#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 12 bytes; isnil +25, recovered from binary.
using Node = legacy::vc71::tree_node<12>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0054b020(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, true>(self, node);
}

HOOK(0x0054B020, StlTree_0054b020, "stl");
