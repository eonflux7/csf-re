#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_C_00865aa8_Vtable, 0x00865AA8);

static C_00865aa8* __fastcall C_00865aa8_Construct(
    C_00865aa8* self, void* /*edx*/)
{
    self->field_0028 = 15;
    self->field_0024 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0014) = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0030 = 0;
    self->field_004c = 1;
    self->field_0050 = 0x3F800000;
    self->vtbl = &g_C_00865aa8_Vtable;
    self->field_0058 = 0.0f;
    self->field_0060 = 0.0f;
    self->field_005c = 0.0f;
    self->field_0054 = 0.0f;
    self->field_0064 = 0;
    return self;
}

HOOK(0x0072A220, C_00865aa8_Construct, "auto");
