#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 4 bytes; isnil +17, recovered from binary.
using Node = legacy::vc71::tree_node<4>;
static Node* __cdecl StlTree_00732ce0(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x00732CE0, StlTree_00732ce0, "stl");
