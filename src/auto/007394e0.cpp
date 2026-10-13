#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 60 bytes; isnil +73, recovered from binary.
using Node = legacy::vc71::tree_node<60>;
static Node* __cdecl StlTree_007394e0(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x007394E0, StlTree_007394e0, "stl");
