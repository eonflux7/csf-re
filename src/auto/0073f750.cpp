#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clears the back-reference held in a spawn object's slot at +4 and releases it.
// Returns the previous pointer value (zero if none was held).
static int __fastcall ReleaseSpawnObjectReference(void* self, void* /*edx*/) {
    uint32_t held = CSF_AT(uint32_t, self, 0x4);
    if (held != 0) {
        CSF_AT(uint32_t, reinterpret_cast<void*>(held), 0x8) = 0;
    }
    CSF_AT(uint32_t, self, 0x4) = 0;
    return static_cast<int>(held);
}

HOOK(0x0073F750, ReleaseSpawnObjectReference, "auto");
