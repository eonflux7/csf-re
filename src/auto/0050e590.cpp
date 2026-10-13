#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 4 bytes; isnil +17, recovered from binary.
using Node = legacy::vc71::tree_node<4>;
static Node* __cdecl StlTree_0050e590(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x0050E590, StlTree_0050e590, "stl");
