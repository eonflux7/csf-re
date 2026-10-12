#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a C_008673b0 texture view to its default state, stores a2 in field_0004,
// and returns the object pointer.
static uint32_t* __fastcall C_008673b0_InitializeTextureView(C_008673b0* self, void* /*edx*/, int a2)
{
    self->vtbl = CSF_VTBL(0x008673B0);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0.0f;
    self->field_0020 = 0.0f;
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0008 = 0;
    self->field_0034 = 0;
    // Packed byte writes at +0x38..+0x3F (bytes FF 00 00 80 / 00 FF 00 80 little-endian).
    self->field_0038 = 0x800000FFu;   // == -2147483393 as int32
    self->field_003c = 0x8000FF00u;   // == -2147418368 as int32
    self->field_0040 = 0.0f;
    self->field_0044 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x739110, C_008673b0_InitializeTextureView, "auto");
