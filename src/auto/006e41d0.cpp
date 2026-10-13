#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorOffset, 0x007F8340, (uint32_t* a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorMultiplier, 0x007F8390, (uint32_t* a1, int a2));

// Applies the multiplier and offset color pair (a2[0..1]) to the Rt2d object held in *a1.
// The original pushes the object and color values onto the stack without popping between
// the two cdecl calls, so the offset call receives the first object and first color as its
// trailing arguments. Both are reproduced here with the values read before the first call.
static uint32_t* __cdecl Im3d_SetObjectColorMultiplierAndOffset(uint32_t* a1, uint32_t* a2)
{
    uint32_t* firstObject = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1[0]));
    int multiplierColor = static_cast<int>(a2[0]);

    Rt2dObjectSetColorMultiplier(firstObject, multiplierColor);

    int offsetColor = static_cast<int>(a2[1]);
    uint32_t* currentObject = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1[0]));
    Rt2dObjectSetColorOffset(currentObject, offsetColor,
                             static_cast<int>(reinterpret_cast<uintptr_t>(firstObject)),
                             multiplierColor);

    return a1;
}

HOOK(0x6E41D0, Im3d_SetObjectColorMultiplierAndOffset, "auto");
