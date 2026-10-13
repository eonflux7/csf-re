#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 4 bytes; isnil +17, recovered from binary.
using Node = legacy::vc71::tree_node<4>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0050e530(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, true>(self, node);
}

HOOK(0x0050E530, StlTree_0050e530, "stl");
