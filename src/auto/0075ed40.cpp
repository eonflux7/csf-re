#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 629: value 36 bytes; isnil +49, recovered from binary.
using Node = legacy::vc71::tree_node<36>;
static Node* __cdecl StlTree_0075ed40(Node* node) {
    return legacy::vc71::tree_extreme<Node, false>(node);
}

HOOK(0x0075ED40, StlTree_0075ed40, "stl");
