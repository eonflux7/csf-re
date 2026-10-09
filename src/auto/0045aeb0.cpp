#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x004E8C50: __cdecl, no arguments, returns the manager object pointer.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// 0x004DC5F0: __thiscall-style (ecx = this) with one stack argument; returns -1 when the lookup misses.
CSF_FUNC(int, __fastcall, LookupEntryIndex, 0x004DC5F0, (void* self, void* edx, float* a2));

// Stores 1 at owner+1 when the lookup of (a3->inner->[4] + 4) in the manager's sub-table misses (-1), else 0.
// Returns a1 unchanged.
static int __stdcall SetLookupMissFlag(int a1, int /*a2*/, int a3)
{
    int keyBase = *reinterpret_cast<int*>(*reinterpret_cast<int*>(a3) + 4);

    int manager = GetValue8B0554();
    int subTable = *reinterpret_cast<int*>(manager + 0x1FD8);

    void* tableSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(subTable) + 0x34);
    int lookupIndex = LookupEntryIndex(tableSelf, nullptr,
                                       reinterpret_cast<float*>(static_cast<uintptr_t>(keyBase + 4)));

    uint8_t missFlag = (lookupIndex == -1) ? 1 : 0;

    uint8_t* selfBase = reinterpret_cast<uint8_t*>(a1);
    uint8_t* ownerBase = *CSF_AT(uint8_t**, selfBase, 0x04);
    *CSF_AT(uint8_t*, ownerBase, 0x01) = missFlag;

    return a1;
}

HOOK(0x0045AEB0, SetLookupMissFlag, "auto");
