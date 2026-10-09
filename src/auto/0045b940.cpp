#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(uint32_t, __fastcall, ResolveHandleWithGlobal, 0x00441220, (void* self, void* edx, uint32_t* a3, uint32_t arg4));

static int __stdcall ResolvePairHandlesWithGlobal(int a1, int a2, uint32_t* a3)
{
    // a3 is a pair of pointers. Follow each pointer's +4 link to reach the inner records.
    uint32_t* firstObj = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3[0]));
    uint32_t* firstInner = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(firstObj[1]));
    uint32_t* firstSlot = &firstInner[1];

    uint32_t* secondObj = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3[1]));
    uint32_t* secondInner = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(secondObj[1]));
    uint32_t secondValue = secondInner[1];

    int globalObject = GetGlobal8afc7c();
    return static_cast<int>(ResolveHandleWithGlobal(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalObject)),
        nullptr,
        firstSlot,
        secondValue));
}

HOOK(0x0045B940, ResolvePairHandlesWithGlobal, "auto");
