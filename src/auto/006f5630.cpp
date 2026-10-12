#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: C_0086200c initializer, __thiscall (ecx = this), returns uint32_t*.
CSF_FUNC(uint32_t*, __fastcall, C_0086200c_Initialize, 0x006EC240, (C_0086200c* self, void* edx));

// Constructor for C_008627d8: installs the vtable, zeroes the first three
// scalar fields, sets field_0010 to 10, initializes the embedded sub-object
// at +0x14, and returns this.
static uint32_t* __fastcall C_008627d8_Construct(C_008627d8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008627D8);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    C_0086200c_Initialize(reinterpret_cast<C_0086200c*>(&self->field_0014), nullptr);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006F5630, C_008627d8_Construct, "auto");
