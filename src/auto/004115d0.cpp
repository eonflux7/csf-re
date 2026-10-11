#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (void* self, void* edx, int a2));

static int __fastcall C_0082df28_UpdateState(C_0082df28* self, void* /*edx*/, int a2)
{
    bool v3;

    // First virtual probe on a2->obj (vtbl slot 32): only when a2's flag byte is clear.
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char probeResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, obj, 0)[32])(
                obj, nullptr, 0, a2);
        v3 = (probeResult == 0);
    }
    else
    {
        v3 = false;
    }

    int savedField = CSF_AT(int, a2, 0x1c);
    CSF_AT(uint8_t, a2, 0) = v3 ? 1 : 0;

    StateNode_UpdateBase(self, nullptr, a2);

    bool flagClear = (CSF_AT(uint8_t, a2, 0) == 0);
    CSF_AT(int, a2, 0x1c) = savedField;

    if (flagClear)
    {
        // Reload obj after the call; the callee may have changed a2.
        void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char finishResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[31])(
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

HOOK(0x004115D0, C_0082df28_UpdateState, "auto");
