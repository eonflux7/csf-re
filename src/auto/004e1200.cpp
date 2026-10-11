#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0083e160 constructor: sets secondary vtables, two stack args stored at +0x0C (a2) and +0x08 (a3),
// clears +0x10 and sets +0x14 to -1. Returns the object address (&vtbl).
static uint32_t* __fastcall C_0083e160_Construct(C_0083e160* self, void* /*edx*/, int a2, int a3)
{
    // Base-class vtable stored first (overwritten below, but still part of the original behavior).
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D428);
    self->vtbl = CSF_VTBL(0x0083E160);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0083E144);
    self->field_000c = (uint32_t)a2;
    self->field_0008 = (uint32_t)a3;
    self->field_0014 = (uint32_t)-1;
    self->field_0010 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4E1200, C_0083e160_Construct, "auto");
