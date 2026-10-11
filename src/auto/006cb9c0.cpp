#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the stored value at +0xA8 only when the flag byte at +0x96 is set; otherwise 0.
static int __fastcall C_00860bb8_GetValueIfEnabled(C_00860bb8* self, void* /*edx*/) {
    if (self->field_0096) {
        return static_cast<int>(self->field_00a8);
    }
    return 0;
}

HOOK(0x006CB9C0, C_00860bb8_GetValueIfEnabled, "auto");
