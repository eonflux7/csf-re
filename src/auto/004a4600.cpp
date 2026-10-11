#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 75 when the inner object's flag at +0x1C4 equals 1; otherwise returns `this`.
static int __fastcall C_00834598_GetStatusCodeOrSelf(C_00834598* self, void* /*edx*/) {
    int result = 75;
    uint8_t* inner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(int, inner, 0x1c4) != 1) {
        return reinterpret_cast<int>(self);
    }
    return result;
}

HOOK(0x4A4600, C_00834598_GetStatusCodeOrSelf, "auto");
