#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_C_008657e0Vtable, 0x008657E0);

static C_008657e0* __fastcall C_008657e0_Initialize(
    C_008657e0* self, void* /*edx*/)
{
    self->field_0028 = 15;
    self->field_0024 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0014) = 0;
    self->vtbl = &g_C_008657e0Vtable;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0030 = 0;
    self->field_004c = 1;
    self->field_0050 = 1.0f;
    return self;
}

HOOK(0x0072A170, C_008657e0_Initialize, "auto");
