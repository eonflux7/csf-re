#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 32 bytes; isnil +45, recovered from binary.
using Node = legacy::vc71::tree_node<32>;
static Node* __cdecl StlTree_0043e090(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x0043E090, StlTree_0043e090, "stl");
