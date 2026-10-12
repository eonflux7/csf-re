#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall C_00867358 ctor-like initializer: sets vtable, zeroes state,
// stores a2 at +0x24, sets the two float-valued fields (1.0f and 127.0f bit patterns).
// Returns this as std::uint32_t*.
static uint32_t* __fastcall C_00867358_InitializeTextureView(C_00867358* self, void* /*edx*/, int a2)
{
    self->vtbl = CSF_VTBL(0x867358);
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0x3f800000;   // 1.0f
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = static_cast<uint32_t>(a2);
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0x42fe0000;   // 127.0f

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x738AF0, C_00867358_InitializeTextureView, "auto");
