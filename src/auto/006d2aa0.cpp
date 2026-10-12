#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Returns the float at +0x6C of the object referenced by this+0x0C, or g_TransitionOne when that reference is null.
static double __fastcall CCamera_GetReferencedTransitionMetric(void* self, void* /*edx*/) {
    uint32_t referencedAddress = CSF_AT(uint32_t, self, 0x0C);
    if (referencedAddress != 0) {
        uint8_t* referencedObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(referencedAddress));
        return static_cast<double>(CSF_AT(float, referencedObject, 0x6C));
    }
    return static_cast<double>(g_TransitionOne);
}

HOOK(0x006D2AA0, CCamera_GetReferencedTransitionMetric, "auto");
