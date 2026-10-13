#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 60 bytes; isnil +73, recovered from binary.
using Node = legacy::vc71::tree_node<60>;
static Node* __cdecl StlTree_007394c0(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x007394C0, StlTree_007394c0, "stl");
