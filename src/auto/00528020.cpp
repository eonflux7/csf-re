#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets every field of a C_008411a0 sound instance to its default state,
// installs the base vtable and the embedded C_0082D448 vtable, and returns this.
static C_008411a0* __fastcall C_008411a0_InitDefaults(C_008411a0* self, void* /*edx*/)
{
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x0082D448);
    self->field_002c = 0xFFFFFFFFu;
    self->field_0030 = 0xFFFFFFFFu;
    self->field_0014 = 0.0f;
    self->field_0018 = 0;
    self->field_0038 = 0;
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    self->field_0034 = 0;
    self->field_0004 = 1;
    self->field_0008 = 1;
    self->field_0039 = 1;
    self->field_003a = 1;
    self->field_003c = 0;
    self->field_0060 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0050 = 0.0f;
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0054 = 0;
    self->vtbl = CSF_VTBL(0x008411A0);
    self->field_0058 = 1.0f;
    self->field_005c = 1.0f;
    return self;
}

HOOK(0x00528020, C_008411a0_InitDefaults, "auto");
