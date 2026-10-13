#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the float at +0x28 of the object referenced by this+0x10, or 0.0 when that reference is null.
static double __fastcall GfxObject_GetLinkedFloatOrZero(void* self, void* /*edx*/) {
    uint32_t linkedAddress = CSF_AT(uint32_t, self, 0x10);
    if (linkedAddress != 0) {
        uint8_t* linkedObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedAddress));
        return static_cast<double>(CSF_AT(float, linkedObject, 0x28));
    }
    return static_cast<double>(g_fZero);
}

HOOK(0x006B8320, GfxObject_GetLinkedFloatOrZero, "auto");
