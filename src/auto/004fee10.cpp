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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Sends a name (inline buffer at +4 or pointer at +4) to the manager's sink via vtable slot 3.
// If that is not possible or fails (-1), sends the global default value at +0x900 via slot 4.
// Sets *outFlag to 1 when the fallback send succeeds.
static int __cdecl SceneLoader_SendNameWithFallback(void* nameHolder, uint8_t* outFlag)
{
    *outFlag = 0;

    void* manager = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* sink = CSF_AT(void*, manager, 0x1FD4);

    int result = -1;
    bool haveName = nameHolder != nullptr && CSF_AT(uint32_t, nameHolder, 0x14) != 0;

    if (haveName) {
        void* namePtr;
        if (CSF_AT(uint32_t, nameHolder, 0x18) < 0x10u)
            namePtr = static_cast<void*>(static_cast<uint8_t*>(nameHolder) + 4);
        else
            namePtr = CSF_AT(void*, nameHolder, 4);

        result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, sink, 0)[3])(sink, nullptr, namePtr);
    }

    if (!haveName || result == -1) {
        void* globalBase = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
        uint32_t defaultValue = CSF_AT(uint32_t, globalBase, 0x900);

        result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, sink, 0)[4])(sink, nullptr, reinterpret_cast<void*>(static_cast<uintptr_t>(defaultValue)));

        if (result != -1)
            *outFlag = 1;
    }

    return result;
}

HOOK(0x4FEE10, SceneLoader_SendNameWithFallback, "auto");
