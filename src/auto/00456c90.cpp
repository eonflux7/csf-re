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

// Constructor for C_008312e0 (vtable 0x8312E0). Stores the vtable, sets up the
// two data-table pointers in field_0004, zeroes the counters and sets both
// limit fields (+0x1C, +0x2C) to 10. Returns the object pointer.
static uint32_t* __fastcall C_008312e0_Construct(C_008312e0* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D428);
    self->vtbl = CSF_VTBL(0x008312E0);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x008312C4);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 10;
    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00456C90, C_008312e0_Construct, "auto");
