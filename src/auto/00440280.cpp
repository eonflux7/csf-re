#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 17: value 32 bytes, color +44; helper boundaries retained.
using Node = legacy::vc71::tree_node<32>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __stdcall, BuyNode, 0x0043F8E0, (Node* left, Node* parent, Node* right, const void* value, uint8_t color));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0043E1F0, (Tree* self, void* edx, Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0043E0D0, (Tree* self, void* edx, Node* node));
struct Operations {
    using Node = ::Node;
    static __forceinline Node* buy(Node* left, Node* parent, Node* right, const void* value, uint8_t color) { return BuyNode(left, parent, right, value, color); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
};
static Node** __fastcall StlTree_00440280(Tree* self, void* edx, Node** result, uint8_t left, Node* parent, const void* value) {
    return legacy::vc71::tree_insert<Operations>(self, edx, result, left, parent, value);
}
HOOK(0x00440280, StlTree_00440280, "stl");
