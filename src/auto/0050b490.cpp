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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));

CSF_GLOBAL(int, g_Value_8BB450, 0x008BB450);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static C_00860130* __fastcall OptionsFile_SetScaledValueAndApply(float* self, void* /*edx*/, float value)
{
    CSF_AT(float, self, 0x10) = value;

    if (g_Value_8BB450 == 0)
        return nullptr;

    int globalId = GetGlobalInt();
    void* obj = LookupObjectById(reinterpret_cast<void*>(static_cast<intptr_t>(globalId)), nullptr);

    int scaledValue = static_cast<int>(CSF_AT(float, self, 0x10) * g_Const200_0f);

    // vtable slot 0x4C / 4 = 19
    auto applyFn = reinterpret_cast<C_00860130* (__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[19]);
    return applyFn(obj, nullptr, scaledValue);
}

HOOK(0x0050B490, OptionsFile_SetScaledValueAndApply, "auto");
