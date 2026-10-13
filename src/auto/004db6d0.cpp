#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the linked sub-object's virtual slot 18 (byte offset 0x48).
// The sub-object is at this+0xC, and its own linked object is at +0x10.
// The slot is called with the linked object as `this` and the sub-object as its
// single argument. Returns the slot's result, or 0 if there is no sub-object.
static int __stdcall AiStateAccessors_ForwardToLinkedSlot18(int a1)
{
    void* owner = reinterpret_cast<void*>(a1);
    void* subObject = CSF_AT(void*, owner, 0xC);
    if (subObject == nullptr)
        return 0;

    void* target = CSF_AT(void*, subObject, 0x10);
    using SlotFn = int(__fastcall*)(void* self, void* edx, void* arg);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[18]);
    return slot(target, nullptr, subObject);
}

HOOK(0x4DB6D0, AiStateAccessors_ForwardToLinkedSlot18, "auto");
