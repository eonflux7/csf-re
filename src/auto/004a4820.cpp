#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the object's state/type field (+0x44) is 1, 3 or 23; otherwise 0.
static int __fastcall C_00834598_IsTypeOneThreeOr23(C_00834598* self, void* /*edx*/) {
    uint32_t typeValue = self->field_0044;
    if (typeValue == 1 || typeValue == 3 || typeValue == 23) {
        return 1;
    }
    return 0;
}

HOOK(0x4A4820, C_00834598_IsTypeOneThreeOr23, "auto");
