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

// CHandleTable::Resolve (__thiscall, one stack arg, ret 4)
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Updates the target state from a resolved handle table entry.
// __thiscall with 3 stack args (a2, a3, a4), so ret 12.
static int __fastcall UpdateResolvedHandleTarget(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint32_t sourceEntry = CSF_AT(uint32_t, *a4, 4);
    int result = CSF_AT(int, sourceEntry, 12);
    uintptr_t handleSlot = static_cast<uintptr_t>(sourceEntry) + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleSlot));
        if (result)
        {
            int targetObject = CSF_AT(int, result, 0x34);
            if (targetObject)
            {
                int callArgs[2];
                callArgs[0] = 10;
                callArgs[1] = 0;
                uint32_t requestSource = CSF_AT(uint32_t, a4[1], 4);
                callArgs[1] = CSF_AT(int, requestSource, 4);

                void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(targetObject));
                // vtable slot 0xA8 / 4 = 42, __thiscall with one stack arg
                ((void(__fastcall*)(void*, void*, int*))(CSF_AT(void**, obj, 0)[42]))(obj, nullptr, callArgs);

                result = CSF_AT(int, handleSlot, 4);
                CSF_AT(int, self, 0x60) = result;
                CSF_AT(int, self, 0x64) = CSF_AT(int, handleSlot, 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045EEF0, UpdateResolvedHandleTarget, "auto");
