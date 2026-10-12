#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Bound callees (addresses from the packet).
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Sets bit 0 of the flags word at +0x680 on the object linked to `self` (offset -4 from the
// linked value), then forwards to UnnamedApply_4DC180 with the linked object's sub-object
// at +0x1C4 (or the constant 0x1C8 when no linked object exists).
static int __stdcall AiMoveTasks_SetLinkedFlagAndApply(int self)
{
    void* selfPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(self)));

    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(selfPtr, nullptr);
    uint8_t* flaggedObject = nullptr;
    if (linkedObjectValue != 0)
        flaggedObject = reinterpret_cast<uint8_t*>(linkedObjectValue - 4);
    CSF_AT(uint32_t, flaggedObject, 0x680) |= 1u;

    int applyArgument = 0;
    int linkedAgain = GetLinkedObjectValue_4DB5C0(selfPtr, nullptr);
    if (linkedAgain != 0)
        applyArgument = linkedAgain - 4 + 456;
    else
        applyArgument = 456;

    return UnnamedApply_4DC180(selfPtr, nullptr, applyArgument);
}

HOOK(0x004AAFE0, AiMoveTasks_SetLinkedFlagAndApply, "auto");
