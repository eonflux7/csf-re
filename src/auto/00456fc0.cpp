#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static C_00831304* __fastcall C_00831304_InitializeDefaults(
    C_00831304* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00831304);
    self->field_0004 = 0xFFFFFFFFu;
    self->field_0020 = 15;
    self->field_001c = 0;
    self->field_000c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    return self;
}

HOOK(0x00456FC0, C_00831304_InitializeDefaults, "auto");
