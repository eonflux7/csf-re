#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 609: value 36 bytes; isnil +49, recovered from binary.
using Node = legacy::vc71::tree_node<36>;
static Node* __cdecl StlTree_0075ed20(Node* node) {
    return legacy::vc71::tree_extreme<Node, true>(node);
}

HOOK(0x0075ED20, StlTree_0075ed20, "stl");
