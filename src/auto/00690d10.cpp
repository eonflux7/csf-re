#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Ors a byte argument into the flag byte at +0x28, after shifting the existing
// flags left by one bit, and returns the new flag byte.
static char __fastcall C_00861a00_ShiftFlagsAndOrByte(C_00861a00* self, void* /*edx*/, char flagBits) {
    char updatedFlags = static_cast<char>(flagBits | (CSF_AT(uint8_t, self, 0x28) << 1));
    CSF_AT(char, self, 0x28) = updatedFlags;
    return updatedFlags;
}

HOOK(0x690D10, C_00861a00_ShiftFlagsAndOrByte, "auto");
