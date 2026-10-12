#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Returns the float at +0x68 of the object referenced by this+0x0C, or 1.0 when that reference is null.
static double __fastcall CCamera_GetSkinFloatMetric(void* self, void* /*edx*/) {
    uint32_t referencedAddress = CSF_AT(uint32_t, self, 0x0C);
    if (referencedAddress != 0) {
        uint8_t* referencedObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(referencedAddress));
        return static_cast<double>(CSF_AT(float, referencedObject, 0x68));
    }
    return static_cast<double>(g_TransitionOne);
}

HOOK(0x006D2A80, CCamera_GetSkinFloatMetric, "auto");
