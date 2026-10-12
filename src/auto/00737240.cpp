#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores a2 into the object's field at +0x10 only when it differs from the
// current value, and returns a2. __thiscall with one stack argument, ret 4.
static int __fastcall C_00867260_SetField10IfChanged(C_00867260* self, void* /*edx*/, int newValue) {
    if (newValue != CSF_AT(int, self, 0x10)) {
        CSF_AT(int, self, 0x10) = newValue;
    }
    return newValue;
}

HOOK(0x00737240, C_00867260_SetField10IfChanged, "auto");
