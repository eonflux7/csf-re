#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall SetSignedInputMagnitude(void* self, void* /*edx*/, float amount)
{
    if (amount == g_fZero)
        return;

    if (amount > g_fZero)
    {
        CSF_AT(uint8_t, self, 0x10C) = 1;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            CSF_AT(float, self, 0xE0) =
                CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x188) * amount;
        }
        else
        {
            CSF_AT(float, self, 0xE0) =
                CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x15C) * amount;
        }
    }
    else
    {
        CSF_AT(uint8_t, self, 0x10D) = 1;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            CSF_AT(float, self, 0xE4) =
                -(CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x18C) * amount);
        }
        else
        {
            CSF_AT(float, self, 0xE4) =
                -(CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x160) * amount);
        }
    }
}

HOOK(0x004117B0, SetSignedInputMagnitude, "auto");
