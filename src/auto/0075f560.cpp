#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 17: value 36 bytes, color +48; helper boundaries retained.
using Node = legacy::vc71::tree_node<36>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __stdcall, BuyNode, 0x0075EF60, (Node* left, Node* parent, Node* right, const void* value, uint8_t color));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0075EF00, (Tree* self, void* edx, Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0075ED60, (Tree* self, void* edx, Node* node));
struct Operations {
    using Node = ::Node;
    static __forceinline Node* buy(Node* left, Node* parent, Node* right, const void* value, uint8_t color) { return BuyNode(left, parent, right, value, color); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
};
static Node** __fastcall StlTree_0075f560(Tree* self, void* edx, Node** result, uint8_t left, Node* parent, const void* value) {
    return legacy::vc71::tree_insert<Operations>(self, edx, result, left, parent, value);
}
HOOK(0x0075F560, StlTree_0075f560, "stl");
