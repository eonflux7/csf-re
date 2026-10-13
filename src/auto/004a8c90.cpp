#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeBaseState, 0x004D19C0, (S_0041f0a0* self, void* , S_0041f0a0* archive));

// Refreshes the state byte of archive `a2` from its owner object, serializes the base state
// of `self` into `a2`, then re-evaluates the state byte through the owner's query slots.
// Returns 1 when the state byte ends up set, 0 when the owner requested a stop.
// a2 layout: +0x00 state byte, +0x04 owner object (vtable-bearing), +0x15/+0x16 scratch bytes,
// +0x1C saved dword.
static int __fastcall S_0041f0a0_UpdateArchiveFlagAndSerialize(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2)
{
    bool newFlag = false;
    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, a2, 0x04);
        bool ownerBlocked = ((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, a2);
        newFlag = !ownerBlocked;
    }

    uint32_t savedField1C = CSF_AT(uint32_t, a2, 0x1C);
    CSF_AT(uint8_t, a2, 0x00) = newFlag ? 1 : 0;

    S_0041f0a0_SerializeBaseState(self, nullptr, a2);

    CSF_AT(uint32_t, a2, 0x1C) = 0;
    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, a2, 0x04);
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        void* memberSlot = reinterpret_cast<uint8_t*>(self) + 0x3C;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, S_0041f0a0*))
            CSF_AT(void**, owner, 0)[0x58 / 4])(owner, nullptr, memberSlot, a2);
        CSF_AT(uint8_t, a2, 0x00) = (queryResult == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, a2, 0x1C) = savedField1C;
    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, a2, 0x04);
        bool ownerRequestsStop = ((bool(__fastcall*)(void*, void*, S_0041f0a0*))
            CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, a2);
        if (ownerRequestsStop)
        {
            CSF_AT(uint8_t, a2, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0x00) = 1;
    return 1;
}

HOOK(0x004A8C90, S_0041f0a0_UpdateArchiveFlagAndSerialize, "auto");
