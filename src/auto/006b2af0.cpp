#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, GfxObjectInstances_UpdateLeafState, 0x006E5370, (float* self, void* /*edx*/, float* position));

// Walks slot 44 (+0xB0) from self to the deepest node, then asks that node's
// slot 7 (+0x1C) for a child and that child's slot 4 (+0x10) for a float*, and
// forwards it to GfxObjectInstances_UpdateLeafState with the leaf's +0x84 as this.
// Returns 0xFF when the leaf has no child.
static char __fastcall GfxObject_ProcessDeepestLeaf(void* self, void* /*edx*/)
{
    void* deepest = self;
    void* next = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[44])(self, nullptr);
    while (next) {
        deepest = next;
        next = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, deepest, 0)[44])(deepest, nullptr);
    }

    void* leafChild = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, deepest, 0)[7])(deepest, nullptr);
    if (!leafChild) {
        return static_cast<char>(0xFF);
    }

    float* position = reinterpret_cast<float*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, leafChild, 0)[4])(leafChild, nullptr));
    float* leafState = reinterpret_cast<float*>(CSF_AT(void*, deepest, 0x84));
    return GfxObjectInstances_UpdateLeafState(leafState, nullptr, position);
}

HOOK(0x006B2AF0, GfxObject_ProcessDeepestLeaf, "auto");
