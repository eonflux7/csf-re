#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Returns whether the signed 32-bit field at offset 0x1BC of the object is
// non-negative (>= 0). The original zeroes eax, tests the value, and uses
// setge al, so the result is a 32-bit int/BOOL rather than a byte bool.
static int __fastcall IsField1BCNonNegative(void* self, void* /*edx*/)
{
    int value = CSF_AT(int, self, 0x1bc);
    return value >= 0;
}

HOOK(0x492B10, IsField1BCNonNegative, "auto");
