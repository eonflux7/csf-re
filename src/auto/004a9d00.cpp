#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, AiState_SerializeInner, 0x004DC0B0, (void* self, void* edx, int a2));

static int __fastcall AiMoveTasks_SerializeBaseState(void* self, void* /*edx*/, int a1)
{
    bool v1;

    // First virtual probe on a1->obj (vtbl slot 32): only when a1's flag byte is clear.
    if (CSF_AT(uint8_t, a1, 0) == 0)
    {
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a1, 4));
        unsigned char probeResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, obj, 0)[32])(
                obj, nullptr, 0, a1);
        v1 = (probeResult == 0);
    }
    else
    {
        v1 = false;
    }

    int savedField = CSF_AT(int, a1, 0x1c);
    CSF_AT(uint8_t, a1, 0) = v1 ? 1 : 0;

    AiState_SerializeInner(self, nullptr, a1);

    bool flagClear = (CSF_AT(uint8_t, a1, 0) == 0);
    CSF_AT(int, a1, 0x1c) = savedField;

    if (flagClear)
    {
        // Reload obj after the call; the callee may have changed a1.
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a1, 4));
        unsigned char finishResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[31])(
                obj, nullptr, a1);
        if (finishResult != 0)
        {
            CSF_AT(uint8_t, a1, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a1, 0) = 1;
    return 1;
}

HOOK(0x004A9D00, AiMoveTasks_SerializeBaseState, "auto");
