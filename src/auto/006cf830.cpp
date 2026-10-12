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

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Constructor for the collision shape (SPHERE/ELLIPSOID) class: runs the base
// constructor, zeroes the shape's own fields, then installs the derived vtable.
static int __fastcall C_00860cbc_Construct(C_00860cbc* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->vtbl = CSF_VTBL(0x00860CBC);
    return reinterpret_cast<int>(self);
}

HOOK(0x006CF830, C_00860cbc_Construct, "auto");
