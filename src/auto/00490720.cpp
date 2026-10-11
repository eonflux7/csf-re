#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_48FA00, 0x0048FA00, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_48FA70, 0x0048FA70, (void* self, void* edx));

static unsigned int __fastcall C_00833908_UpdateStateFlags(
    C_00833908* self, void* /*edx*/)
{
    const bool enabled = self->vGetField016c() != 0
        && (self->field_0094 & 0x12) == 0;

    if (enabled)
        self->field_0170 |= 1;
    else
        self->field_0170 &= 0xFFFFFFFE;

    const bool active = self->vGetField016c() != 0;
    if (active)
        self->field_0170 |= 2;
    else
        self->field_0170 &= 0xFFFFFFFD;

    sub_48FA00(self, nullptr);
    return sub_48FA70(self, nullptr);
}

HOOK(0x00490720, C_00833908_UpdateStateFlags, "auto");
