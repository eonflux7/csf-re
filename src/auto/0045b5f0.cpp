#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(char, __fastcall, ApplyEntryToProvider, 0x004F55A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Resolves the cached object provider from the global value chain, applies the
// value read through a4's pointer chain to it, then clears the flag at +0x82 and
// sets +0x18 to 7. Returns the char result of ApplyEntryToProvider.
static char __fastcall ProcessEntryWithCachedProvider(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a2;
    (void)a3;

    const int globalValue = GetGlobalValue_4FBDE0();
    const int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* cachedProvider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);

    // a4 points to a pointer chain: *(*(*(a4) + 4) + 4)
    const int* a4Ref = reinterpret_cast<const int*>(a4);
    const int* stage1 = reinterpret_cast<const int*>(*a4Ref);
    const int* stage2 = reinterpret_cast<const int*>(stage1[1]);
    const int entryValue = stage2[1];

    const char result = ApplyEntryToProvider(cachedProvider, nullptr, entryValue);

    CSF_AT(uint8_t, self, 0x82) = 0;
    CSF_AT(int, self, 0x18) = 7;
    return result;
}

HOOK(0x0045B5F0, ProcessEntryWithCachedProvider, "auto");
