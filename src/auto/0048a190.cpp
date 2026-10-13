#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor of C_00833708 (vtable 0x833708, 28 bytes). Stores a primary vtable
// and a secondary-base vtable (the +4 slot is written twice, as in the original),
// zeroes three state fields, sets field_0018 to 10, and keeps a2 in field_0008.
static uint32_t* __fastcall C_00833708_Construct(C_00833708* self, void* /*edx*/, int a2) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D428);
    self->vtbl = CSF_VTBL(0x00833708);
    self->field_0004 = (uint32_t)CSF_VTBL(0x008336E8);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_0008 = (uint32_t)a2;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0048A190, C_00833708_Construct, "auto");
