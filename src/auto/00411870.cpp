#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall SetSignedAxisMagnitude(void* self, void* /*edx*/, float amount)
{
    if (amount == g_fZero)
        return;

    if (amount < g_fZero)
    {
        CSF_AT(uint8_t, self, 0x10E) = 1;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            CSF_AT(float, self, 0xE8) =
                -(CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x190) * amount);
        }
        else
        {
            CSF_AT(float, self, 0xE8) =
                -(CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x164) * amount);
        }
    }
    else
    {
        CSF_AT(uint8_t, self, 0x10F) = 1;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            CSF_AT(float, self, 0xEC) =
                CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x190) * amount;
        }
        else
        {
            CSF_AT(float, self, 0xEC) =
                CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x164) * amount;
        }
    }
}

HOOK(0x00411870, SetSignedAxisMagnitude, "auto");
