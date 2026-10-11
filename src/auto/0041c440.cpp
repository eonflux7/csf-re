#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e280_PrepareWheelQuery, 0x0041B360, (void* self, void* edx, int a2));

// Updates the wheel side-state record `ctx` for the vehicle `self`.
// ctx layout (unnamed struct): +0x00 state byte, +0x04 owner object (vtable-bearing),
// +0x15/+0x16 scratch bytes, +0x1C saved dword.
static int __fastcall C_0082e280_UpdateWheelSideState(C_0082e280* self, void* /*edx*/, void* ctx)
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

    C_0082e280_PrepareWheelQuery(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(ctx)));

    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* wheelPosition = reinterpret_cast<uint8_t*>(self) + 0x8C;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr, wheelPosition, ctx);
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

HOOK(0x0041C440, C_0082e280_UpdateWheelSideState, "auto");
