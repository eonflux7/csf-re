#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CMenuMessageBox_ResetState(
    CMenuMessageBox* self, void* /*edx*/)
{
    self->field_0078 = UINT32_MAX;
    self->field_0082 = 0;
    self->field_0086 = 0;
    self->field_0087 = 0;
    self->field_0088 = 0;
    self->field_0089 = 0;
    self->field_008a = 0;
    self->field_008b = 0;
    self->field_008c = 0;
    self->field_008d = 0;
    self->field_008e = 0;
    self->field_008f = 0;
    self->field_0083 = 0;
    self->field_0084 = 0;
    self->field_0085 = 0;
    self->field_0080 = 1;
    self->field_0081 = 0;
    self->field_0998 = 0;
    self->field_099c = 0;
    self->field_09a4 = 0;
    self->field_09a8 = 0;
    self->field_0898 = 0;
    self->field_089c = 0;
    return 0;
}

HOOK(0x005288F0, CMenuMessageBox_ResetState, "auto");
