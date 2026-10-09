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
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_4F5180, 0x004F5180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_761410, 0x00761410, (char* String1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Reads a string from the record passed in a3, stores it in a temporary string,
// resolves its id and forwards it to the cached object provider.
static int __stdcall SetCachedProviderNameFromArgString(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    CGuiControl* cachedProvider = reinterpret_cast<CGuiControl*>(
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr));

    // a3 -> record pointer; record+4 -> inner record; inner+4 -> source text address.
    std::uint32_t* outerRef = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(a3));
    std::uint32_t* innerRecord = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(*outerRef));
    std::uintptr_t sourceAddress = static_cast<std::uintptr_t>(innerRecord[1] + 4);

    C_0082d410 stringHolder;
    CSF_AT(std::uint32_t, &stringHolder, 0x18) = 15;   // capacity
    CSF_AT(std::uint32_t, &stringHolder, 0x14) = 0;    // length
    stringHolder.field_0004 = 0;                       // low byte cleared in original

    AssignStringSubstring(&stringHolder, nullptr,
                          reinterpret_cast<const void*>(sourceAddress), 0, 0xFFFFFFFFu);

    char* textPtr = reinterpret_cast<char*>(stringHolder.field_0004);
    if (CSF_AT(std::uint32_t, &stringHolder, 0x18) < 0x10)
        textPtr = reinterpret_cast<char*>(&stringHolder.field_0004);

    int nameId = sub_761410(textPtr);
    int result = sub_4F5180(cachedProvider, nullptr, nameId);

    if (CSF_AT(std::uint32_t, &stringHolder, 0x18) >= 0x10) {
        result = static_cast<int>(stringHolder.field_0004);
        if (result != 0)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x464E00, SetCachedProviderNameFromArgString, "auto");
