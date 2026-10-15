#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"
using Node = legacy::vc71::tree_node<8>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(void, __fastcall, Advance, 0x00665E40, (Node** self, void* edx));
CSF_FUNC(Node*, __cdecl, Minimum, 0x0069A630, (Node* node));
CSF_FUNC(Node*, __cdecl, Maximum, 0x0069A610, (Node* node));
CSF_FUNC(void, __fastcall, RotateLeft, 0x0069A650, (Tree* self, void* edx, Node* node));
CSF_FUNC(void, __fastcall, RotateRight, 0x0069A6B0, (Tree* self, void* edx, Node* node));
CSF_FUNC(void, __cdecl, Free, 0x00690E60, (void* block));
struct Operations {
    using Node = ::Node;
    static __forceinline void advance(Node** p) { Advance(p, nullptr); }
    static __forceinline Node* min(Node* p) { return Minimum(p); }
    static __forceinline Node* max(Node* p) { return Maximum(p); }
    static __forceinline void lrotate(Tree* t, Node* p) { RotateLeft(t, nullptr, p); }
    static __forceinline void rrotate(Tree* t, Node* p) { RotateRight(t, nullptr, p); }
    static __forceinline void destroy(Node* p) { Free(p); }
};
static Node** __fastcall EraseNodeFromRange(Tree* self, void* edx, Node** result, Node* position) {
    return legacy::vc71::tree_erase<Operations>(self, edx, result, position);
}
HOOK(0x006B7670, EraseNodeFromRange, "stl");
