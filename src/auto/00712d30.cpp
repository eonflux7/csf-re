#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CInputBase constructor body: installs the base vtable and zeroes the input state.
// Returns the object pointer (the vtable slot sits at offset 0).
static uint32_t* __fastcall CInputBase_InitDefaults(CInputBase* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x863E58);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    self->field_0018 = 0;
    self->field_0004 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x712D30, CInputBase_InitDefaults, "auto");
