#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"


static C_00865870* __fastcall C_00865870_Initialize(
    C_00865870* self, void* /*edx*/)
{
    self->field_0028 = 15;
    self->field_0024 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0014) = 0;
    self->vtbl = reinterpret_cast<void**>(CSF_VTBL(0x00865870));
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0x3f800000;
    self->field_0030 = 0;
    self->field_0044 = 0.0f;
    self->field_0048 = 0;
    return self;
}

HOOK(0x0072A1B0, C_00865870_Initialize, "auto");
