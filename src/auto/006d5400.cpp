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

// C_00860178::C_00860178 (base constructor), __fastcall with ret 0.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Constructor of C_00861068: runs the base constructor, installs this class's
// vtable, zeroes five 4-byte fields at +0x08..+0x18 and sets +0x1C to 1.0f.
// Returns this in eax.
static C_00861068* __fastcall C_00861068_Construct(C_00861068* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00861068);
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(float, self, 0x1C) = 1.0f;
    return self;
}

HOOK(0x006D5400, C_00861068_Construct, "auto");
