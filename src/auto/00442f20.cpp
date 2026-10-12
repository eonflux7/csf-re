#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_00831074_Initialize(C_00831074* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00831074);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    reinterpret_cast<uint8_t*>(&self->field_0004)[0] = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00442F20, C_00831074_Initialize, "auto");
