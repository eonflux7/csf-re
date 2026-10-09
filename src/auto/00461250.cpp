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

static uint32_t ReadDwordAt(uint32_t address) {
    return *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(address));
}

static void* AsObjectPointer(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

// Resolves the handle stored in the record at *a3, calls the resolved object's
// virtual chain, and forwards a value read from a3[1] to the final object.
static int __stdcall ResolveHandleAndForwardValue(int /*a1*/, int /*a2*/, uint32_t* a3) {
    uint32_t recordAddress = a3[0];
    uint32_t v3 = ReadDwordAt(recordAddress + 4);
    int32_t flagsOrIndex = static_cast<int32_t>(ReadDwordAt(v3 + 12));
    int result = static_cast<int>(v3 + 4);

    if (flagsOrIndex >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0) {
            void* resolvedObject = AsObjectPointer(static_cast<uint32_t>(result));

            // slot 0x28 / 4 = 10
            int v6 = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolvedObject, 0)[10])(resolvedObject, nullptr);
            void* secondObject = AsObjectPointer(static_cast<uint32_t>(v6));

            // slot 0x50 / 4 = 20
            int v7 = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, secondObject, 0)[20])(secondObject, nullptr);
            void* thirdObject = AsObjectPointer(static_cast<uint32_t>(v7));

            // slot 0x18 / 4 = 6
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, thirdObject, 0)[6])(thirdObject, nullptr);

            // slot 4 / 4 = 1, argument = *(*(a3[1] + 4) + 4)
            uint32_t sourceRecord = a3[1];
            uint32_t sourceInner = ReadDwordAt(sourceRecord + 4);
            int forwardedValue = static_cast<int>(ReadDwordAt(sourceInner + 4));

            return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, thirdObject, 0)[1])(thirdObject, nullptr, forwardedValue);
        }
    }
    return result;
}

HOOK(0x00461250, ResolveHandleAndForwardValue, "auto");
