#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeBaseState, 0x004D19C0, (S_0041f0a0* self, void* , S_0041f0a0* archive));

// Updates the side-state record `ctx` for the object `self`.
// ctx layout (unnamed struct): +0x00 state byte, +0x04 owner object (vtable-bearing),
// +0x15/+0x16 scratch bytes, +0x1C saved dword.
static int __fastcall S_0041f0a0_UpdateSideStateRecord(S_0041f0a0* self, void* /*edx*/, void* ctx)
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

    S_0041f0a0_SerializeBaseState(self, nullptr, reinterpret_cast<S_0041f0a0*>(ctx));

    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* childPosition = reinterpret_cast<uint8_t*>(self) + 0x3C;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x44 / 4])(owner, nullptr, childPosition, ctx);
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

HOOK(0x004D6710, S_0041f0a0_UpdateSideStateRecord, "auto");
