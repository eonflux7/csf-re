#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores newValue at +0x78 only when it differs from the current value, and returns newValue.
static int __fastcall C_00867260_SetField78IfChanged(C_00867260* self, void* /*edx*/, int newValue) {
    int result = newValue;
    if (CSF_AT(int, self, 0x78) != newValue)
        CSF_AT(int, self, 0x78) = newValue;
    return result;
}

HOOK(0x007370F0, C_00867260_SetField78IfChanged, "auto");
