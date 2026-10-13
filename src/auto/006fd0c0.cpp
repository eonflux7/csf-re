#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stores a flag byte at this+0x80, sets the byte at this+0x81 to 1,
// and returns the stored flag byte (al). The class id is not identified,
// so the object is addressed through a raw pointer.
static char __fastcall LightObject_StoreFlagAndSetDirty(void* self, void* /*edx*/, char a2) {
    CSF_AT(char, self, 0x80) = a2;
    CSF_AT(char, self, 0x81) = 1;
    return a2;
}

HOOK(0x006FD0C0, LightObject_StoreFlagAndSetDirty, "auto");
