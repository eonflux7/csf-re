#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears the low byte of the object's header word (the flag byte at +0), stores
// the argument as a dword at +4, then sets the flag byte again. Always returns 1.
static char __fastcall C_0083e124_SetGuardedValue(C_0083e124* self, void* /*edx*/, int a2) {
    if (CSF_AT(uint8_t, self, 0) != 0) {
        CSF_AT(uint8_t, self, 0) = 0;
    }
    CSF_AT(int, self, 0x4) = a2;
    CSF_AT(uint8_t, self, 0) = 1;
    return 1;
}

HOOK(0x004DF7E0, C_0083e124_SetGuardedValue, "auto");
