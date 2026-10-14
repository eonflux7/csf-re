#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_tree.h"

// The value is 32 bytes BEFORE the color/nil flags: +0x2C/+0x2D.
using Node = legacy::vc71::tree_node<32>;
using Tree = legacy::vc71::tree<Node>;
CSF_FUNC(Node*, __cdecl, Maximum, 0x0043E090, (Node* node));
CSF_FUNC(Node*, __cdecl, Minimum, 0x0043E0B0, (Node* node));
CSF_FUNC(Node*, __fastcall, Rrotate, 0x0043E0D0, (Tree* self, void*, Node* node));
CSF_FUNC(int*, __fastcall, Advance, 0x0043E190, (int* self, void*));
CSF_FUNC(Node*, __fastcall, Lrotate, 0x0043E1F0, (Tree* self, void*, Node* node));
CSF_FUNC(char, __cdecl, Free, 0x00690E60, (void* block));

struct Operations {
    using Node = ::Node;
    static __forceinline void advance(Node** position) {
        Advance(reinterpret_cast<int*>(position), nullptr);
    }
    static __forceinline Node* min(Node* node) { return Minimum(node); }
    static __forceinline Node* max(Node* node) { return Maximum(node); }
    static __forceinline void lrotate(Tree* self, Node* node) { Lrotate(self, nullptr, node); }
    static __forceinline void rrotate(Tree* self, Node* node) { Rrotate(self, nullptr, node); }
    static __forceinline void destroy(Node* node) {
        // Embedded string starts at +0x0C: buffer +0x10, length +0x20,
        // capacity +0x24. Its storage is released before the node itself.
        if (CSF_AT(uint32_t, node, 0x24) >= 16) {
            void* block = CSF_AT(void*, node, 0x10);
            if (block) Free(block);
        }
        CSF_AT(uint32_t, node, 0x24) = 15;
        CSF_AT(uint32_t, node, 0x20) = 0;
        CSF_AT(uint8_t, node, 0x10) = 0;
        Free(node);
    }
};

// ECX=this, EDX only forwarded on the error path; two stack arguments, ret 8.
// The returned iterator is the advanced position, not the erased node.
static Node** __fastcall EraseTreeNodeFromRange(Tree* self, void* edx,
                                               Node** result, Node* position) {
    return legacy::vc71::tree_erase<Operations>(self, edx, result, position);
}
HOOK(0x0043EFC0, EraseTreeNodeFromRange, "stl");
