#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 8 bytes; isnil +21, recovered from binary.
using Node = legacy::vc71::tree_node<8>;
static Node* __cdecl StlTree_0069a610(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x0069A610, StlTree_0069a610, "stl");
