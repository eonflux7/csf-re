#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Thiscall body: if the pending byte at +0x14 is set, clear it and forward a
// notification to the owner's sub-object through vtable slot 0x8c (35).

static char __fastcall TaskStates_ConsumePendingFlagAndNotify(void* self, void* /*edx*/)
{
    char flag = CSF_AT(char, self, 0x14);
    if (!flag)
        return flag;

    uint32_t ownerAddr = CSF_AT(uint32_t, self, 8);
    void* owner = reinterpret_cast<void*>(ownerAddr);

    // Clear the pending flag before the virtual call, as the original does.
    CSF_AT(char, self, 0x14) = 0;

    void* subObject = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    int stateArg = CSF_AT(int, owner, 0x238);

    using NotifyFn = char(__fastcall*)(void* self, void* edx, int stateArg, int zeroA, int minusOne, int zeroB);
    NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, subObject, 0)[0x8c / 4]);
    return notify(subObject, nullptr, stateArg, 0, -1, 0);
}

HOOK(0x421220, TaskStates_ConsumePendingFlagAndNotify, "auto");
