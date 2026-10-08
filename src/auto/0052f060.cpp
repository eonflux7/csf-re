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

// Cdecl helpers called by the original, bound by address.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

// __thiscall callee: this in ecx, three stack args, callee pops 12 bytes.
CSF_FUNC(int, __fastcall, LookupScenarioEntry, 0x00558340,
         (void* self, void* edx, uint32_t selectedIndex, int paramA, int paramB));

// Returns 0 when the owned sub-object at +0x30 is absent. Otherwise it asks the
// owner (virtual slot 0x2AC) for a selected index, reads the global state and
// the table at globalState+8, looks up the entry using fields +0x40 and +0x44,
// stores it through the out pointer, and returns 1.
static char __fastcall MenuScenarioSelect_TryQueryEntry(void* self, void* /*edx*/, uint32_t* out)
{
    void* owner = CSF_AT(void*, self, 0x30);
    if (!owner)
        return 0;

    // Virtual call: owner->vtbl[0x2AC / 4](owner), __thiscall with no args.
    uint32_t selectedIndex = (uint32_t)((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x2AC / 4])(owner, nullptr);

    // Global state, then the side call whose result is unused.
    int globalState = GetGlobalValue_558150();
    GetValue8B0554();

    // lea ecx, [globalState + 8]: pointer to the sub-object at +8.
    void* lookupTable = reinterpret_cast<void*>(static_cast<uintptr_t>(globalState) + 8);

    int paramA = CSF_AT(int, self, 0x40);
    int paramB = CSF_AT(int, self, 0x44);
    int entry = LookupScenarioEntry(lookupTable, nullptr, selectedIndex, paramA, paramB);

    *out = (uint32_t)entry;
    return 1;
}

HOOK(0x0052F060, MenuScenarioSelect_TryQueryEntry, "auto");
