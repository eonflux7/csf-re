#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constant data referenced by the original: the class vtable at 0x008656D0.

// Constructor for C_008656d0 (vtable 0x008656D0): initialises the vtable pointer
// and the remaining fields without calling any base constructor.
static C_008656d0* __fastcall C_008656d0_Construct(C_008656d0* self, void* /*edx*/)
{
    self->field_0028 = 0xF;
    self->field_0024 = 0;
    // The original only writes the low byte of field_0014 (mov byte ptr [eax+0x14], cl).
    reinterpret_cast<uint8_t*>(&self->field_0014)[0] = 0;
    self->vtbl = CSF_VTBL(0x8656D0);
    self->field_0030 = 0;
    return self;
}

HOOK(0x72A340, C_008656d0_Construct, "auto");
