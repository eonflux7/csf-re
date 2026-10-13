#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the float at +0x2C of the object referenced by this+0x10, or 0.0 when that reference is null.
static double __fastcall GfxObject_GetFloatAt2COrZero(void* self, void* /*edx*/) {
    uint32_t referencedAddress = CSF_AT(uint32_t, self, 0x10);
    if (referencedAddress != 0) {
        uint8_t* referencedObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(referencedAddress));
        return static_cast<double>(CSF_AT(float, referencedObject, 0x2C));
    }
    return static_cast<double>(g_fZero);
}

HOOK(0x006B8340, GfxObject_GetFloatAt2COrZero, "auto");
