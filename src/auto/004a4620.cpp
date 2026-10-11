#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Returns `this` when the object referenced by field_0008 has a nonzero dword at +0x1C4,
// otherwise returns 38 (0x26).
static int __fastcall C_00834598_SelfIfInnerFlagElse38(C_00834598* self, void* /*edx*/) {
    uint8_t* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint32_t innerFlag = CSF_AT(uint32_t, inner, 0x1c4);
    if (innerFlag != 0) {
        return static_cast<int>(reinterpret_cast<uintptr_t>(self));
    }
    return 38;
}

HOOK(0x4A4620, C_00834598_SelfIfInnerFlagElse38, "auto");
