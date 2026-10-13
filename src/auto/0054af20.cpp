#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 12 bytes; isnil +25, recovered from binary.
using Node = legacy::vc71::tree_node<12>;
static Node* __cdecl StlTree_0054af20(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x0054AF20, StlTree_0054af20, "stl");
