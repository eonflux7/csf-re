#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 89: value 36 bytes; isnil +49, recovered from binary.
using Node = legacy::vc71::tree_node<36>;
using Tree = legacy::vc71::tree<Node>;
static Node* __fastcall StlTree_0075ef00(Tree* self, void*, Node* node) {
    return legacy::vc71::tree_rotate<Node, true>(self, node);
}

HOOK(0x0075EF00, StlTree_0075ef00, "stl");
