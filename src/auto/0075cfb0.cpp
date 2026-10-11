#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0075CE20, __fastcall-style binding for the __thiscall-like finder.
CSF_FUNC(int, __fastcall, Table_FindIndex, 0x0075CE20, (void* self, void* edx, int a2));

// Original: 0x00462A80, __thiscall method on S_00692b80.
CSF_FUNC(int, __fastcall, S_00692b80_FindIndexOfValue, 0x00462A80, (S_00692b80* self, void* edx, int value));

// Returns the address of the point record matching groupId/pointId inside the
// group table, or 0 when either lookup fails.
// Layout: group table entries are 72 bytes; each entry holds a sub-table at +0x08
// (as pointer-sized field) and a point array at +0x38 with 44-byte records.
static int __fastcall C_0086c288_GetGroupPoint(C_0086c288* self, void* /*edx*/, int a2, int a3)
{
    int index = Table_FindIndex(self, nullptr, a2);
    if (index == -1)
        return 0;

    uint8_t* groupEntry = reinterpret_cast<uint8_t*>(self->field_0008) + index * 72;

    int valueIndex = S_00692b80_FindIndexOfValue(reinterpret_cast<S_00692b80*>(groupEntry), nullptr, a3);
    if (valueIndex == -1)
        return 0;

    int pointArrayBase = CSF_AT(int, groupEntry, 0x38);
    return pointArrayBase + valueIndex * 44;
}

HOOK(0x75CFB0, C_0086c288_GetGroupPoint, "auto");
