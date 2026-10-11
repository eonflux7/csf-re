#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies the source byte into the flag byte at +7 of the skin object and
// writes back the dword at +4 unchanged, then returns the object pointer.
static int __cdecl SkinCopyFlagByteFromSource(int skinObject, uint8_t* flagSource) {
    uint8_t* self = reinterpret_cast<uint8_t*>(skinObject);
    CSF_AT(uint8_t, self, 0x7) = *flagSource;
    uint32_t word4 = CSF_AT(uint32_t, self, 0x4);
    CSF_AT(uint32_t, self, 0x4) = word4;
    return skinObject;
}

HOOK(0x006C4DD0, SkinCopyFlagByteFromSource, "auto");
