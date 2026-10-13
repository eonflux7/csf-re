#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 12 bytes; isnil +25, recovered from binary.
using Node = legacy::vc71::tree_node<12>;
static Node* __cdecl StlTree_0054af00(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x0054AF00, StlTree_0054af00, "stl");
