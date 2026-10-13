#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// Cluster 17: value 4 bytes, color +16; helper boundaries retained.
using Node = legacy::vc71::tree_node<4>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __stdcall, BuyNode, 0x00733B70, (Node* left, Node* parent, Node* right, const void* value, uint8_t color));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0050E530, (Tree* self, void* edx, Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0050E5B0, (Tree* self, void* edx, Node* node));
struct Operations {
    using Node = ::Node;
    static __forceinline Node* buy(Node* left, Node* parent, Node* right, const void* value, uint8_t color) { return BuyNode(left, parent, right, value, color); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
};
static Node** __fastcall StlTree_00735760(Tree* self, void* edx, Node** result, uint8_t left, Node* parent, const void* value) {
    return legacy::vc71::tree_insert<Operations>(self, edx, result, left, parent, value);
}
HOOK(0x00735760, StlTree_00735760, "stl");
