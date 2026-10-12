#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_0086200c_Initialize(C_0086200c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0086200C);
    self->field_0004 = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;
    self->field_0018 = 1;
    self->field_0019 = 0;
    self->field_0038 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006EC240, C_0086200c_Initialize, "auto");
