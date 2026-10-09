#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Sound_RefreshOutputState, 0x00528610, (void* self, void* edx, void* a2));

static int __fastcall C_00840ff0_UpdateSoundInstanceState(C_00840ff0* self, void* /*edx*/, void* a2)
{
    bool v3;

    // First virtual probe on a2->obj (vtbl slot 32): only when a2's flag byte is clear.
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char probeResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, void*)>(CSF_AT(void**, obj, 0)[32])(
                obj, nullptr, 0, a2);
        v3 = (probeResult == 0);
    }
    else
    {
        v3 = false;
    }

    uint32_t savedField = CSF_AT(uint32_t, a2, 0x1c);
    CSF_AT(uint8_t, a2, 0) = v3 ? 1 : 0;

    Sound_RefreshOutputState(self, nullptr, a2);

    bool flagClear = (CSF_AT(uint8_t, a2, 0) == 0);
    CSF_AT(uint32_t, a2, 0x1c) = savedField;

    if (flagClear)
    {
        // Reload obj after the call; the callee may have changed a2.
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char finishResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, void*)>(CSF_AT(void**, obj, 0)[31])(
                obj, nullptr, a2);
        if (finishResult != 0)
        {
            CSF_AT(uint8_t, a2, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x00528780, C_00840ff0_UpdateSoundInstanceState, "auto");
