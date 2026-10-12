#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Leaf-state submitter: takes a float* from the leaf's child and forwards it,
// with the leaf's field 0x84 as `this`, to the original routine at 0x6E5370.
CSF_FUNC(char, __fastcall, GfxSubmitLeafState, 0x006E5370, (void* self, void* edx, float* a2));

static int __fastcall GfxObject_ProcessDeepestLeaf(void* self, void* /*edx*/)
{
    // Walk the chain through vtable slot 44 (+0xB0) until it yields null.
    void* leaf = self;
    void* next = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, leaf, 0)[44])(leaf, nullptr);
    while (next) {
        leaf = next;
        next = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, leaf, 0)[44])(leaf, nullptr);
    }

    // Slot 7 (+0x1C) on the leaf yields a child object.
    void* child = CSF_AT(void**, leaf, 0)[7] == nullptr
        ? nullptr
        : ((void* (__fastcall*)(void*, void*))CSF_AT(void**, leaf, 0)[7])(leaf, nullptr);
    if (!child) {
        return 255;
    }

    // Slot 4 (+0x10) on the child yields a float* payload.
    float* payload = (float*)((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[4])(child, nullptr);

    // The leaf's field at +0x84 becomes `this` for the submitter.
    void* stateOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, leaf, 0x84));
    return GfxSubmitLeafState(stateOwner, nullptr, payload);
}

HOOK(0x006B2AF0, GfxObject_ProcessDeepestLeaf, "auto");
