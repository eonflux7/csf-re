#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x006E6B30 (9 bytes, ret 0)
// Clears the two byte flags at +0x38 and +0x39, returns 0.
static char __fastcall C_00861d48_ClearByteFlags(C_00861d48* self, void* /*edx*/)
{
    self->field_0038 = 0;
    self->field_0039 = 0;
    return 0;
}

HOOK(0x006E6B30, C_00861d48_ClearByteFlags, "auto");
