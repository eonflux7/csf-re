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

CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));
CSF_FUNC(char, __fastcall, FrameApplyToMatch, 0x006BF380, (void* self, void* edx, int a2));

// Stack search block: [0] = value being searched for, [1] = frame found by the callback.
// The callback writes through the pointer, so both slots live in one 8-byte stack object.
static char __fastcall C_00860700_FindMatchingChildFrame(C_00860700* self, void* /*edx*/, int a2, int a3)
{
    alignas(8) int searchState[2];
    searchState[0] = a2;
    searchState[1] = 0;

    RwFrameForAllChildren(static_cast<int>(self->field_000c),
                          reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006BF2B0)),
                          reinterpret_cast<int>(searchState));

    if (searchState[1] == 0)
        return 0;

    FrameApplyToMatch(reinterpret_cast<void*>(a3), nullptr, searchState[1]);
    return 1;
}

HOOK(0x006BF7E0, C_00860700_FindMatchingChildFrame, "auto");
