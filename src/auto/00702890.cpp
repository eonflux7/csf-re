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

// Base-class constructor C_00860178 (fastcall, this in ecx, edx unused).
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Initializes a C_00863180 object: runs the base constructor, installs the
// class vtable, and zeroes the working fields (field_001c starts at 10).
static C_00863180* __fastcall C_00863180_Construct(C_00863180* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00863180);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    self->field_000c = 0;
    self->field_0004 = 0;
    return self;
}

HOOK(0x00702890, C_00863180_Construct, "auto");
