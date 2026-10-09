#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_48E350, 0x0048E350, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Reads an integer metric from the handle-bound object referenced by a3, passes it
// to the object resolved from the handle entry, and stores the returned value as a
// float into the field at [a1 + 4] + 4.
static int __stdcall UpdateHandleBoundFloatFieldFromMetric(int a1, int a2, uint32_t* a3)
{
    (void)a2;

    // Source metric: float at *(*(a3[1]) + 4) + 4, truncated to int.
    uint8_t* sourceWrapper = reinterpret_cast<uint8_t*>(a3[1]);
    uint8_t* sourceObject = CSF_AT(uint8_t*, sourceWrapper, 4);
    int sourceIntegerValue = static_cast<int>(CSF_AT(float, sourceObject, 4));

    // Handle entry slot: *(a3[0] + 4) + 4.
    uint8_t* holder = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* handleEntry = CSF_AT(uint8_t*, holder, 4) + 4;

    int result = CSF_AT(int, handleEntry, 8);
    if (result >= 0)
    {
        int resolvedHandle = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(handleEntry)));
        result = resolvedHandle;
        if (resolvedHandle != 0)
        {
            uint8_t* selfObject = reinterpret_cast<uint8_t*>(a1);
            uint8_t* targetObject = CSF_AT(uint8_t*, selfObject, 4);

            result = sub_48E350(reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedHandle)),
                                nullptr, sourceIntegerValue);

            // Signed int -> float, with the unsigned bias added for negative values.
            float storedValue = static_cast<float>(result);
            if (result < 0)
                storedValue += g_UnsignedIntegerBias;
            CSF_AT(float, targetObject, 4) = storedValue;
        }
    }
    return result;
}

HOOK(0x0045C4A0, UpdateHandleBoundFloatFieldFromMetric, "auto");
