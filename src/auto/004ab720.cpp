#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound callees (addresses from the packet)
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Resets the linked object's field at +0x6DC to zero, then applies the task
// stored at linked object + 0x570 (the field 1392 bytes in) to the owner.
static int __stdcall AiMoveTasks_ResetLinkedFlagAndApplyTask(void* self)
{
    // First: clear the flag on the linked object (base = linked - 4, or 0 if none).
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedBase = nullptr;
    if (linkedObjectValue)
        linkedBase = reinterpret_cast<uint8_t*>(linkedObjectValue - 4);
    CSF_AT(int, linkedBase, 0x6DC) = 0;

    // Second: apply the task at +0x570 of the linked object (or +0x570 from null).
    int linkedObjectValueAgain = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linkedObjectValueAgain)
        return UnnamedApply_4DC180(self, nullptr, linkedObjectValueAgain - 4 + 0x570);
    return UnnamedApply_4DC180(self, nullptr, 0x570);
}

HOOK(0x004AB720, AiMoveTasks_ResetLinkedFlagAndApplyTask, "auto");
