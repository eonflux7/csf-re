#include <cstdint>
#include "globals.h"
#include "hook.h"

// Gfx frame record layout (no declared class): +0x00 state byte, +0x04 target object,
// +0x15/+0x16 flag bytes, +0x1C saved word. The target's vtable slots are read via CSF_AT.

static int __fastcall GfxFrames_UpdateFrameRecordState(void* self, void* /*edx*/, void* rec)
{
    // v8 = rec->field_1C
    const int savedWord = CSF_AT(int, rec, 0x1C);

    // v4 = !rec->state && !target->vtbl[32](0, rec)
    bool resultFlag = false;
    if (CSF_AT(uint8_t, rec, 0x00) == 0)
    {
        void* target = CSF_AT(void*, rec, 0x04);
        bool probeResult = reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, target, 0)[32])(target, nullptr, 0, rec);
        resultFlag = !probeResult;
    }
    CSF_AT(uint8_t, rec, 0x00) = resultFlag ? 1 : 0;
    CSF_AT(int, rec, 0x1C) = 0;

    if (!resultFlag)
    {
        CSF_AT(uint8_t, rec, 0x15) = 0;
        CSF_AT(uint8_t, rec, 0x16) = 0;
        void* target = CSF_AT(void*, rec, 0x04);
        int callResult = reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, target, 0)[5])(target, nullptr, self, rec);
        CSF_AT(uint8_t, rec, 0x00) = (callResult == -1) ? 1 : 0;
    }

    // v6 = (rec->state == 0); restore saved word
    const bool stateClear = CSF_AT(uint8_t, rec, 0x00) == 0;
    CSF_AT(int, rec, 0x1C) = savedWord;

    if (stateClear)
    {
        void* target = CSF_AT(void*, rec, 0x04);
        bool finished = reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, target, 0)[31])(target, nullptr, rec);
        if (finished)
        {
            CSF_AT(uint8_t, rec, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, rec, 0x00) = 1;
    return 1;
}

HOOK(0x006BFFC0, GfxFrames_UpdateFrameRecordState, "auto");
