#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 17: value 8 bytes, color +20; helper boundaries retained.
using Node = legacy::vc71::tree_node<8>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __stdcall, BuyNode, 0x006B6F40, (Node* left, Node* parent, Node* right, const void* value, uint8_t color));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0069A650, (Tree* self, void* edx, Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0069A6B0, (Tree* self, void* edx, Node* node));
struct Operations {
    using Node = ::Node;
    static __forceinline Node* buy(Node* left, Node* parent, Node* right, const void* value, uint8_t color) { return BuyNode(left, parent, right, value, color); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
};
static Node** __fastcall StlTree_006b7ac0(Tree* self, void* edx, Node** result, uint8_t left, Node* parent, const void* value) {
    return legacy::vc71::tree_insert<Operations>(self, edx, result, left, parent, value);
}
HOOK(0x006B7AC0, StlTree_006b7ac0, "stl");
