#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the linked sub-object's virtual slot 15 (byte offset 0x3C).
// The sub-object is at this+0xC, and its own linked object is at +0x10.
// Returns 0 if there is no linked sub-object.
static int __stdcall AiStateAccessors_ForwardToLinkedSlot15(int a1)
{
    void* owner = reinterpret_cast<void*>(a1);
    void* subObject = CSF_AT(void*, owner, 0xC);
    if (subObject == nullptr)
        return 0;

    void* target = CSF_AT(void*, subObject, 0x10);
    using SlotFn = int(__fastcall*)(void* self, void* edx, void* arg);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[15]);
    return slot(target, nullptr, subObject);
}

HOOK(0x4DB670, AiStateAccessors_ForwardToLinkedSlot15, "auto");
