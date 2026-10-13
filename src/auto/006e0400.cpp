#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the byte flag at +0x8, stores C_008619e0's vtable at +0x0, and clears
// the flag if it was set. Returns the flag value read before the clear.
static char __fastcall C_008619e0_ResetVtableAndClearFlag(C_008619e0* self, void* /*edx*/)
{
    char flagValue = CSF_AT(char, self, 0x8);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x008619E0);
    if (flagValue)
        CSF_AT(char, self, 0x8) = 0;
    return flagValue;
}

HOOK(0x6E0400, C_008619e0_ResetVtableAndClearFlag, "auto");
