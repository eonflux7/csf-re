#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall S_00692b10_InitializeFile(
    S_00692b10* self, void* /*edx*/)
{
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0014 = 1;
    self->field_0018 = 0;
    self->field_0015 = static_cast<uint8_t>(-1);
    return &self->field_0000;
}

HOOK(0x00692B10, S_00692b10_InitializeFile, "auto");
