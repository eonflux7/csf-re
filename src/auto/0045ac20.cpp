#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Binding addresses of the original functions and data this routine uses.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Sums the weight field (+0x60) of every entry whose +0x38 value is non-negative
// and whose +0x3c flag is zero, then stores the total as a float at result->+4.
// Returns the first argument unchanged.
static int __stdcall SumActiveEntryWeights(int resultHolderArg, int a2, int a3)
{
    (void)a2;
    (void)a3;

    uint32_t totalWeight = 0;

    void* globalValueObject = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* entryList = DefaultDestination_Get(globalValueObject, nullptr);

    for (uint32_t index = 0; index < static_cast<uint32_t>(GetEntryCount(entryList, nullptr)); ++index)
    {
        int entry = GetEntryAt(entryList, nullptr, static_cast<int>(index));
        void* entryPtr = reinterpret_cast<void*>(entry);

        if (CSF_AT(int32_t, entryPtr, 0x38) >= 0 && CSF_AT(int32_t, entryPtr, 0x3C) == 0)
        {
            totalWeight += static_cast<uint32_t>(CSF_AT(int32_t, entryPtr, 0x60));
        }
    }

    // The total is converted as a signed value; when the sign bit is set,
    // the unsigned bias is added to recover the unsigned magnitude.
    float totalAsFloat = static_cast<float>(static_cast<int32_t>(totalWeight));
    if (static_cast<int32_t>(totalWeight) < 0)
    {
        totalAsFloat += g_UnsignedIntegerBias;
    }

    void* outputHolder = CSF_AT(void*, reinterpret_cast<void*>(resultHolderArg), 4);
    CSF_AT(float, outputHolder, 4) = totalAsFloat;

    return resultHolderArg;
}

HOOK(0x0045AC20, SumActiveEntryWeights, "auto");
