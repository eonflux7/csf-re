#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"
using Node = legacy::vc71::tree_node<12>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __cdecl, Maximum, 0x0054AF00, (Node* node));
CSF_FUNC(Node*, __cdecl, Minimum, 0x0054AF20, (Node* node));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0054B020, (Tree* self, void*, Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0054B080, (Tree* self, void*, Node* node));
CSF_FUNC(int*, __fastcall, Advance, 0x0054B290, (int* self, void*));
CSF_FUNC(char, __cdecl, Free, 0x00690E60, (void* block));
struct Operations {
    using Node = ::Node;
    static __forceinline void advance(Node** position) { Advance(reinterpret_cast<int*>(position), nullptr); }
    static __forceinline Node* min(Node* node) { return Minimum(node); }
    static __forceinline Node* max(Node* node) { return Maximum(node); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
    static __forceinline void destroy(Node* node) { Free(node); }
};
// Flags follow the 12-byte value: color +24, isnil +25. Return advanced iterator.
static Node** __fastcall TypeRegistryEraseTreeNode(Tree* self, void* edx, Node** result, Node* position) {
    return legacy::vc71::tree_erase<Operations>(self, edx, result, position);
}
HOOK(0x0054B640, TypeRegistryEraseTreeNode, "stl");
