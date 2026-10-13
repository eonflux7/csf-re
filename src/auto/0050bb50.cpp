#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style reset: installs C_0083fdf4's vtable and zeroes the
// five state fields (+0x04..+0x10 dwords, +0x14 byte). Returns this in eax.
static uint8_t* __fastcall C_0083fdf4_Construct(C_0083fdf4* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083FDF4);
    self->field_0014 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x0050BB50, C_0083fdf4_Construct, "auto");
