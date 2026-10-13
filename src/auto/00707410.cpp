#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00861d48_ClearByteFlags, 0x006E6B30, (C_00861d48* self, void*));
CSF_FUNC(int, __fastcall, sub_707140, 0x00707140, (void* self, void* edx));

// Clears the base-class byte flags, then returns the current sub-object (field_0050)
// when its tag at +0x5C matches field_0054; otherwise hands off to sub_707140.
static int __fastcall C_008632c0_ClearFlagsThenValidate(C_008632c0* self, void* /*edx*/)
{
    C_00861d48_ClearByteFlags(self, nullptr);

    uint32_t result = self->field_0050;
    uint32_t tag = CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x5C);
    if (tag != self->field_0054)
        return sub_707140(self, nullptr);

    return static_cast<int>(result);
}

HOOK(0x707410, C_008632c0_ClearFlagsThenValidate, "auto");
