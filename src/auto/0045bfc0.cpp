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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, C_0083e9d0_ComputeValue, 0x004E6410, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Resolves the handle stored in the record referenced by a3, looks up the scene
// object it refers to, computes a value from that object and stores it as a float
// into the block referenced by a1. Returns the raw computed value (or 0 / the handle
// when the lookup fails, as in the original).
static unsigned int __stdcall UpdateFloatFromSceneHandle(int a1, int a2, int a3)
{
    // a3 points to a pointer to a record; record+4 holds the handle base.
    uint32_t* recordRef = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3));
    void* record = reinterpret_cast<void*>(static_cast<uintptr_t>(*recordRef));
    uint32_t handleBase = CSF_AT(uint32_t, record, 4);

    // Signed flag value stored at handleBase+12 decides whether the lookup happens.
    void* handleBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(handleBase));
    int32_t flagValue = CSF_AT(int32_t, handleBlock, 12);

    unsigned int result = handleBase + 4;
    if (flagValue >= 0)
    {
        result = static_cast<unsigned int>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(result)));
        if (result)
        {
            result = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x3C);
            if (result)
            {
                // Target block pointer is read before the call, as in the original.
                void* argBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
                uint32_t targetBlock = CSF_AT(uint32_t, argBlock, 4);

                result = C_0083e9d0_ComputeValue(reinterpret_cast<void*>(static_cast<uintptr_t>(result)), nullptr);

                int32_t signedResult = static_cast<int32_t>(result);
                float value = static_cast<float>(signedResult);
                if (signedResult < 0)
                {
                    value += g_UnsignedIntegerBias;
                }
                CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(targetBlock)), 4) = value;
            }
        }
    }
    return result;
}

HOOK(0x0045BFC0, UpdateFloatFromSceneHandle, "auto");
