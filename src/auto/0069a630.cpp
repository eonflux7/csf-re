#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 8 bytes; isnil +21, recovered from binary.
using Node = legacy::vc71::tree_node<8>;
static Node* __cdecl StlTree_0069a630(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x0069A630, StlTree_0069a630, "stl");
