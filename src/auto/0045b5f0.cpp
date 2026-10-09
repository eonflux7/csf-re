#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ApplyEntryToProvider, 0x004F55A0, (void* self, void* edx, int a2));

static char __fastcall ProcessEntryWithCachedProvider(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a2;
    (void)a3;

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement((void*)(uintptr_t)globalValue, nullptr);
    void* cachedProvider = GetCachedObjectProvider((void*)(uintptr_t)firstElement, nullptr);

    // a4 points to a pointer chain: *(*(*a4 + 4) + 4)
    uint32_t firstLink = *(uint32_t*)(uintptr_t)a4;
    uint32_t secondLink = *(uint32_t*)(uintptr_t)(firstLink + 4);
    uint32_t entryValue = *(uint32_t*)(uintptr_t)(secondLink + 4);

    char result = ApplyEntryToProvider(cachedProvider, nullptr, (int)entryValue);

    *CSF_AT(uint8_t*, self, 0x82) = 0;
    *CSF_AT(uint32_t*, self, 0x18) = 7;
    return result;
}

HOOK(0x0045B5F0, ProcessEntryWithCachedProvider, "auto");
