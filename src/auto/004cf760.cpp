#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A8C90, 0x004A8C90, (void* self, void* edx, void* a2));

// Refreshes the side-state record `ctx` for the object `self`. The record holds a state byte at +0x00,
// an owner object (vtable-bearing) at +0x04, scratch bytes at +0x15/+0x16 and a saved dword at +0x1C.
// Returns 1 when the state ends up set, 0 when the owner asks to stop.
static int __fastcall S_0041f0a0_UpdateSideStateFromOwner(S_0041f0a0* self, void* /*edx*/, void* ctx)
{
    bool newFlag = false;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        bool ownerBlocked = ((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, ctx);
        newFlag = !ownerBlocked;
    }

    uint32_t savedField1C = CSF_AT(uint32_t, ctx, 0x1C);
    CSF_AT(uint8_t, ctx, 0x00) = newFlag ? 1 : 0;

    sub_4A8C90(self, nullptr, ctx);

    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* sidePosition = reinterpret_cast<uint8_t*>(self) + 0x40;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x44 / 4])(owner, nullptr, sidePosition, ctx);
        CSF_AT(uint8_t, ctx, 0x00) = (queryResult == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, ctx, 0x1C) = savedField1C;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        bool ownerRequestsStop = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, ctx);
        if (ownerRequestsStop)
        {
            CSF_AT(uint8_t, ctx, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, ctx, 0x00) = 1;
    return 1;
}

HOOK(0x004CF760, S_0041f0a0_UpdateSideStateFromOwner, "auto");
