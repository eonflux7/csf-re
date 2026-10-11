#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores a flag byte at this+0x3D, sets the byte at this+0x3E to 1,
// and returns the stored flag byte (al).
static char __fastcall C_0082dd78_StoreByteAndSetFlag(C_0082dd78* self, void* /*edx*/, char a2) {
    CSF_AT(char, self, 0x3d) = a2;
    CSF_AT(char, self, 0x3e) = 1;
    return a2;
}

HOOK(0x0040FD20, C_0082dd78_StoreByteAndSetFlag, "auto");
