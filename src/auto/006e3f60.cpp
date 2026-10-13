#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original calls (bind as given in the packet).
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorOffset, 0x007F8340, (uint32_t* a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorMultiplier, 0x007F8390, (uint32_t* a1, int a2));

// holder: pointer to a slot whose first dword is the Rt2d object handle.
// colorState: record whose +4 is a color offset value and +8 is a state kind.
static uint32_t* __cdecl Im3d_ApplyColorStateToObject(uint32_t* holder, uint8_t* colorState)
{
    int stateKind = CSF_AT(int, colorState, 0x08);

    if (stateKind != 0 && (stateKind <= 6 || stateKind > 10))
    {
        // Special state kinds (1..6 excluded from the range check): reset multiplier and zero offset.
        Rt2dObjectSetColorMultiplier(reinterpret_cast<uint32_t*>(*holder), 0);
        Rt2dObjectSetColorOffset(reinterpret_cast<uint32_t*>(*holder), 0, 0, 0);
        return holder;
    }

    Rt2dObjectSetColorMultiplier(reinterpret_cast<uint32_t*>(*holder), 0);
    Rt2dObjectSetColorOffset(reinterpret_cast<uint32_t*>(*holder),
                             CSF_AT(int, colorState, 0x04), 0, 0);
    return holder;
}

HOOK(0x006E3F60, Im3d_ApplyColorStateToObject, "auto");
