#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// C_008639b0 constructor: resets all audio-effect state to defaults, stamps the
// class vtable, copies one byte from the caller's descriptor (+0x0B), and returns this.
static uint32_t* __fastcall C_008639b0_Construct(C_008639b0* self, void* /*edx*/, int a2)
{
  self->field_0008 = 1.0f;
  self->field_0004 = 1.0f;
  self->field_000c = 0.0f;
  self->field_0014 = 0;
  self->field_001c = 0;
  self->field_0020 = 0;
  self->field_0038 = 0;
  self->field_0039 = 0;
  self->field_003a = 0;
  self->field_003b = 0;
  self->field_0018 = 0;
  self->field_0024 = 0;
  self->field_0030 = 0;
  self->field_0034 = 0;
  self->field_003c = 0;
  self->field_0040 = 0;
  self->field_0041 = 0;
  self->field_0042 = 0;
  self->field_0043 = 0;
  self->field_0044 = 0;
  self->field_0045 = 0;
  self->field_0046 = 0;
  self->field_0010 = 0;
  self->vtbl = CSF_VTBL(0x008639B0);
  self->field_0028 = 2;
  self->field_002c = 2;
  self->field_0047 = CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2)), 0x0B);
  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0070E730, C_008639b0_Construct, "auto");
