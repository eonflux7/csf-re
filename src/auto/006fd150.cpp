#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the float at +0x14 of the RpLight referenced by this+0x0C (its radius), or 0.0 when no light exists.
static double __fastcall CLightObject_GetLightRadius(CLightObject* self, void* /*edx*/) {
    uint32_t lightAddress = CSF_AT(uint32_t, self, 0x0C);
    if (lightAddress != 0) {
        uint8_t* light = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(lightAddress));
        return static_cast<double>(CSF_AT(float, light, 0x14));
    }
    return static_cast<double>(g_fZero);
}

HOOK(0x006FD150, CLightObject_GetLightRadius, "auto");
