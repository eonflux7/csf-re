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

// RpPrtStdPropTabGetPropOffset: returns the byte offset of a property slot in a PrtStd property table.
CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

// Zeroes the first dword of a PrtStd property slot and sets the second to 1.0f.
// a2 is the particle record; its +0x0C field points to an object whose +0x10 field
// is the property table handle used to look up the slot offset (property id 10).
static int __cdecl PrtStd_InitPropSlotToOne(int a1, int a2)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a2, 0x0C));
    uint32_t propTableHandle = CSF_AT(uint32_t, owner, 0x10);

    int propOffset = RpPrtStdPropTabGetPropOffset(static_cast<int>(propTableHandle), 10);

    uint8_t* slot = reinterpret_cast<uint8_t*>(a2) + propOffset;
    CSF_AT(uint32_t, slot, 0x00) = 0;
    CSF_AT(uint32_t, slot, 0x04) = 0x3F800000u; // 1.0f

    return a2;
}

HOOK(0x7038B0, PrtStd_InitPropSlotToOne, "auto");
