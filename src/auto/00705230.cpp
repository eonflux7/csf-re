#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Initializes a C_008631a8 object in place: runs the base constructor, installs the
// derived vtable, and sets default values (1.0f scale fields, -1 / 0x80 byte flags).
static int __fastcall C_008631a8_InitParticleDefaults(C_008631a8* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x008631A8);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_001c = 0x3f800000u;
    self->field_0020 = 0x3f800000u;
    self->field_0028 = 0x3f800000u;
    self->field_0024 = 0x3f800000u;
    self->field_002c = 0x3f800000u;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0031 = 0;
    self->field_0032 = 0;
    self->field_0034 = 0;
    self->field_0036 = 0;
    self->field_0038 = 0;
    self->field_0030 = 0xFF;
    self->field_0033 = 0x80;
    self->field_0035 = 0xFF;
    self->field_0037 = 0x80;

    return (int)self;
}

HOOK(0x00705230, C_008631a8_InitParticleDefaults, "auto");
