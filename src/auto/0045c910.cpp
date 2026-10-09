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
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Writes the resolved target id into the output slot held by the first argument.
// Falls back to the entry's stored default when the handle does not resolve to a
// live, unlocked entry. Returns the value written (or the output slot pointer when
// the tag check fails).
static int __stdcall ResolveHandleTargetIntoState(int stateHolder, int unusedArg, int requestRef)
{
    (void)unusedArg;

    uint8_t* holder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateHolder));
    uint8_t* outputSlot = *reinterpret_cast<uint8_t**>(holder + 4);
    *reinterpret_cast<int32_t*>(outputSlot + 4) = -1;

    uint8_t* requestPtr = *reinterpret_cast<uint8_t**>(static_cast<uintptr_t>(requestRef));
    uint8_t* requestData = *reinterpret_cast<uint8_t**>(requestPtr + 4);
    int32_t requestTag = *reinterpret_cast<int32_t*>(requestData + 12);
    int handleValue = static_cast<int>(reinterpret_cast<uintptr_t>(requestData + 4));

    int result = handleValue;
    if (requestTag >= 0)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue)));

        if (!entry || *reinterpret_cast<int32_t*>(entry + 0x50) != 0
            || *reinterpret_cast<int32_t*>(entry + 0x3A0) == -1)
        {
            // Fallback: take the value stored behind the entry's default pointer.
            uint8_t* defaultPtr = *reinterpret_cast<uint8_t**>(entry + 0x2C);
            result = *reinterpret_cast<int32_t*>(defaultPtr);
            uint8_t* slotNow = *reinterpret_cast<uint8_t**>(holder + 4);
            *reinterpret_cast<int32_t*>(slotNow + 4) = result;
        }
        else
        {
            // Resolved: publish the entry's id.
            int32_t resolvedId = *reinterpret_cast<int32_t*>(entry + 0x3A0);
            uint8_t* slotNow = *reinterpret_cast<uint8_t**>(holder + 4);
            result = static_cast<int>(reinterpret_cast<uintptr_t>(slotNow));
            *reinterpret_cast<int32_t*>(slotNow + 4) = resolvedId;
        }
    }
    return result;
}

HOOK(0x45C910, ResolveHandleTargetIntoState, "auto");
