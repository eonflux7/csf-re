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

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, QueryIndexedValue_4FC160, 0x004FC160, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static unsigned int __stdcall Subobj_StoreGlobalCounterAsFloat(int a1, int a2, int a3)
{
    uint8_t* subObject = CSF_AT(uint8_t*, a1, 4);
    *reinterpret_cast<float*>(subObject + 4) = -1.0f;

    GetGlobalValue_4FBDE0();
    int result = QueryIndexedValue_4FC160(nullptr, nullptr, 1);
    if (result != -1)
    {
        // Treat the returned value as signed; values with the top bit set get the unsigned bias added.
        int32_t signedValue = static_cast<int32_t>(result);
        float asFloat = static_cast<float>(signedValue);
        if (signedValue < 0)
            asFloat += g_UnsignedIntegerBias;

        uint8_t* subObjectAgain = CSF_AT(uint8_t*, a1, 4);
        *reinterpret_cast<float*>(subObjectAgain + 4) = asFloat;
    }
    return static_cast<unsigned int>(result);
}

HOOK(0x45AD20, Subobj_StoreGlobalCounterAsFloat, "auto");
