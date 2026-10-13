#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 32 bytes; isnil +45, recovered from binary.
using Node = legacy::vc71::tree_node<32>;
static Node* __cdecl StlTree_0043e0b0(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x0043E0B0, StlTree_0043e0b0, "stl");
