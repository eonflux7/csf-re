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

// The vtable the constructor installs (address of the vtable array).
CSF_GLOBAL(void*, g_Vtable_00865980, 0x00865980);

// 0x0072A2C0 (46 bytes): constructor / initializer for C_00865980.
// Installs the vtable, writes a default mode/count of 15 at +0x28, a 1.0f scalar
// at +0x34, and zeroes the remaining fields. Returns `this` in eax.
static C_00865980* __fastcall C_00865980_Construct(C_00865980* self, void* /*edx*/)
{
    self->field_0028 = 0xF;
    self->field_0024 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0014) = 0;
    self->vtbl = &g_Vtable_00865980;
    self->field_0030 = 0;
    self->field_0034 = 1.0f;
    self->field_003c = 0;
    self->field_0044 = 0;
    self->field_0040 = 0;
    self->field_0038 = 0;
    return self;
}

HOOK(0x0072A2C0, C_00865980_Construct, "auto");
