#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns a status code for the object reached through field_0008.
// Codes: 64 when the flag at +0x1C4 is set; otherwise 3, 26, or 50 depending on the mode at +0x1D8.
// Any other mode returns the object's own address as an int.
static int __fastcall C_00834598_GetStatusCode(C_00834598* self, void* /*edx*/) {
    uint8_t* sub = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));

    if (CSF_AT(int, sub, 0x1C4) != 0) {
        return 64;
    }

    uint32_t mode = CSF_AT(uint32_t, sub, 0x1D8);
    int result;
    switch (mode) {
    case 0u:
    case 1u:
        result = 3;
        break;
    case 2u:
    case 3u:
        result = CSF_AT(uint32_t, sub, 0x1E0) != 0 ? 50 : 26;
        break;
    default:
        result = static_cast<int>(reinterpret_cast<uintptr_t>(self));
        break;
    }
    return result;
}

HOOK(0x4A4460, C_00834598_GetStatusCode, "auto");
