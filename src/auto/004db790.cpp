#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the linked sub-object's virtual slot 24 (byte offset 0x60).
// The sub-object is at a1+0xC, and its own linked object is at +0x10.
// The slot is called with the linked object as `this` and the sub-object as its argument.
// Returns 0 if there is no linked sub-object.
static int __stdcall AiStateAccessors_ForwardToLinkedSlot24(int a1)
{
    void* owner = reinterpret_cast<void*>(a1);
    void* subObject = CSF_AT(void*, owner, 0xC);
    if (subObject == nullptr)
        return 0;

    void* target = CSF_AT(void*, subObject, 0x10);
    using SlotFn = int(__fastcall*)(void* self, void* edx, void* arg);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[24]);
    return slot(target, nullptr, subObject);
}

HOOK(0x4DB790, AiStateAccessors_ForwardToLinkedSlot24, "auto");
