#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, StateNodeBase_Refresh, 0x004A8C90, (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_UpdateState(void* self, void* /*edx*/, S_0041f0a0* a1)
{
    bool v3;

    // First virtual probe on a1->obj (vtbl slot 32): only when a1's flag byte is clear.
    if (CSF_AT(uint8_t, a1, 0) == 0)
    {
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a1, 4));
        unsigned char probeResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, void*)>(CSF_AT(void**, obj, 0)[32])(
                obj, nullptr, 0, a1);
        v3 = (probeResult == 0);
    }
    else
    {
        v3 = false;
    }

    int savedField = CSF_AT(int, a1, 0x1c);
    CSF_AT(uint8_t, a1, 0) = v3 ? 1 : 0;

    StateNodeBase_Refresh(self, nullptr, a1);

    bool flagClear = (CSF_AT(uint8_t, a1, 0) == 0);
    CSF_AT(int, a1, 0x1c) = savedField;

    if (flagClear)
    {
        // Reload obj after the call; the callee may have changed a1.
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a1, 4));
        unsigned char finishResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, void*)>(CSF_AT(void**, obj, 0)[31])(
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

HOOK(0x004A8AD0, S_0041f0a0_UpdateState, "auto");
