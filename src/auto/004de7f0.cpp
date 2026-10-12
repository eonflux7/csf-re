#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_0083e124: installs the vtable, zeroes the state fields,
// sets two fields to 10, and returns the object pointer (in eax).
static uint32_t* __fastcall C_0083e124_Construct(C_0083e124* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083E124);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 10;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 10;
    self->field_0004 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004DE7F0, C_0083e124_Construct, "auto");
