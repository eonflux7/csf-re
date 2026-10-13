#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004f1a60_ProcessRecordState, 0x004F1A60, (void* self, void* edx, int a2));

// Refreshes the scene-light record `ctx` (a2) for the parser object `self`.
// ctx layout (unnamed struct): +0x00 state byte, +0x04 owner object (vtable-bearing),
// +0x15/+0x16 scratch bytes, +0x1C saved dword.
static int __fastcall S_004f1a60_UpdateSceneLightRecord(S_004f1a60* self, void* /*edx*/, int a2)
{
    // Address reference to the vtable slot the original binds (no runtime effect).
    static_cast<void>(CSF_VTBL(0x004F1A60));

    void* ctx = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));

    bool newFlag = false;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        bool ownerBlocked = ((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, ctx);
        newFlag = !ownerBlocked;
    }

    int savedField1C = CSF_AT(int, ctx, 0x1C);
    CSF_AT(uint8_t, ctx, 0x00) = newFlag ? 1 : 0;

    S_004f1a60_ProcessRecordState(self, nullptr, a2);

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* recordPosition = reinterpret_cast<uint8_t*>(self) + 0x40;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr, recordPosition, ctx);
        CSF_AT(uint8_t, ctx, 0x00) = (queryResult == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedField1C;
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

HOOK(0x004F1C30, S_004f1a60_UpdateSceneLightRecord, "auto");
