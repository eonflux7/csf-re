#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor/initializer for C_008658f8 (vtable 0x008658F8).
// Installs the vtable, then resets the state fields to their defaults and
// returns the object pointer (as the original does in eax).
static C_008658f8* __fastcall C_008658f8_Construct(C_008658f8* self, void* /*edx*/)
{
    self->field_0028 = 0xF;
    self->field_0024 = 0;
    // Original writes a single byte (cl = 0) to this address.
    *reinterpret_cast<uint8_t*>(&self->field_0014) = 0;
    self->vtbl = CSF_VTBL(0x8658F8);
    self->field_0030 = 0;
    self->field_0034 = 1;
    self->field_0038 = 1.0f;
    return self;
}

HOOK(0x0072A1F0, C_008658f8_Construct, "auto");
