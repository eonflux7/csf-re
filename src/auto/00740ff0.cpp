#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Stores a byte at +0x44C only when it changes, then always sets the flag at +0x748 to 1.
// Returns the value that was passed in.
static char __fastcall S_00740ff0_SetByteAndMarkChanged(S_00740ff0* self, void* /*edx*/, char a2) {
    char result = a2;
    if (a2 != self->field_044c) {
        self->field_044c = a2;
    }
    self->field_0748 = 1;
    return result;
}

HOOK(0x740FF0, S_00740ff0_SetByteAndMarkChanged, "auto");
