#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by address (signatures per packet).
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, sub_51F8D0, 0x0051F8D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_51F830, 0x0051F830, (void* self, void* edx, int a2));

// Reads a3->[0]->[+4]->[+4] to get a chain value, runs it through the global
// lookup sub_51F8D0, then passes that result through a second lookup sub_51F830.
static int __stdcall ResolveChainLookupResult(int /*a1*/, int /*a2*/, int a3)
{
    const uintptr_t argAddress = static_cast<uintptr_t>(a3);
    const uintptr_t firstObject = *reinterpret_cast<uintptr_t*>(argAddress);
    const uintptr_t secondObject = *reinterpret_cast<uintptr_t*>(firstObject + 4);
    const int chainValue = *reinterpret_cast<int*>(secondObject + 4);

    const int firstLookup = sub_51F8D0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_51F740())),
        nullptr,
        chainValue);

    return sub_51F830(
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_51F740())),
        nullptr,
        firstLookup);
}

HOOK(0x45B120, ResolveChainLookupResult, "auto");
