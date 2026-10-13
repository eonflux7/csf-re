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

CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorMultiplier, 0x007F8390, (uint32_t* a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorOffset, 0x007F8340, (uint32_t* a1, int a2, int a3, int a4));

// Resets the Rt2d object's color multiplier and sets its color offset from the
// state record. Kinds 1..6 and 11+ use a zero offset; kind 0 and 7..10 use the
// record's value at +4. Returns the holder pointer.
static uint32_t* __cdecl Im3d_ApplyColorStateToObject(uint32_t* holder, uint8_t* stateRecord)
{
    int stateKind = CSF_AT(int, stateRecord, 8);

    if (stateKind != 0 && (stateKind <= 6 || stateKind > 10))
    {
        Rt2dObjectSetColorMultiplier(reinterpret_cast<uint32_t*>(holder[0]), 0);
        // The original leaves the multiplier call's arguments on the stack, so the
        // offset callee's a3/a4 slots hold the object pointer and 0.
        Rt2dObjectSetColorOffset(reinterpret_cast<uint32_t*>(holder[0]), 0,
                                 static_cast<int>(holder[0]), 0);
        return holder;
    }

    Rt2dObjectSetColorMultiplier(reinterpret_cast<uint32_t*>(holder[0]), 0);
    Rt2dObjectSetColorOffset(reinterpret_cast<uint32_t*>(holder[0]),
                             CSF_AT(int, stateRecord, 4),
                             static_cast<int>(holder[0]), 0);
    return holder;
}

HOOK(0x006E3F60, Im3d_ApplyColorStateToObject, "auto");
