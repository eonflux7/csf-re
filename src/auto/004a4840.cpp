#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the first argument matches a4 and this->field_002c is non-zero
// and this->field_0034 matches a3; otherwise 0. Stdcall-style args (ret 12).
static int __fastcall C_00834598_IsEntryMatchingKeys(C_00834598* self, void* /*edx*/,
                                                     int a2, int a3, int a4)
{
    if (a2 == a4 && self->field_002c != 0 &&
        self->field_0034 == static_cast<uint32_t>(a3)) {
        return 1;
    }
    return 0;
}

HOOK(0x4A4840, C_00834598_IsEntryMatchingKeys, "auto");
