#include "types/auto_classes.h"
#include "hook.h"

// Zeroes the menu-state fields of CMenuMain and returns 0.
// The original writes these offsets (in this exact order):
//   0x10, 0x30, 0x1c, 0x20, 0x24, 0x28, 0x14
static int __fastcall CMenuMain_ResetFields(CMenuMain* self, void* /*edx*/)
{
    self->field_0010 = 0;
    self->field_0030 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0014 = 0;
    return 0;
}

HOOK(0x53A660, CMenuMain_ResetFields, "auto");
