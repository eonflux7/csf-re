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

CSF_FUNC(int, __fastcall, C_00861988_LookupRegionObject, 0x006E0230, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ApplySkinRegionSelection, 0x006C57A0, (C_0085e7b8** self, void* , C_0086754c* a2));
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Looks up the region object for this instance, forwards a2 to its slot 6,
// calls slot 8 on it, applies the skin region selection, then invokes slot 3
// on its embedded sub-object (at +0x20) a3 times with the zero vector.
static int __fastcall C_00861988_ApplyRegionAndZeroVectors(C_00861988* self, void* edx, int a2, int a3)
{
    C_0086754c* regionObject = reinterpret_cast<C_0086754c*>(
        static_cast<uintptr_t>(C_00861988_LookupRegionObject(self, nullptr, 6)));
    if (!regionObject)
        return 0;

    // Virtual slot 6 (+0x18) with a2 as its single argument.
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, regionObject, 0)[6])(regionObject, nullptr, a2);

    // Virtual slot 8 (+0x20) with no arguments.
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, regionObject, 0)[8])(regionObject, nullptr);

    int globalValue = GetGlobalValue_6C5760();
    C_0085e7b8_ApplySkinRegionSelection(
        reinterpret_cast<C_0085e7b8**>(static_cast<uintptr_t>(globalValue)),
        nullptr,
        regionObject);

    uint32_t remaining = static_cast<uint32_t>(a3);
    if (remaining == 0)
        return static_cast<int>(reinterpret_cast<uintptr_t>(regionObject));

    do
    {
        void* embedded = reinterpret_cast<uint8_t*>(regionObject) + 0x20;
        // Vtable of the embedded sub-object at +0x20, slot 3 (+0xC).
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, regionObject, 0x20)[3])(embedded, nullptr, &g_ZeroVectorX);
        --remaining;
    } while (remaining != 0);

    return static_cast<int>(reinterpret_cast<uintptr_t>(regionObject));
}

HOOK(0x006E0320, C_00861988_ApplyRegionAndZeroVectors, "auto");
