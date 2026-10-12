#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor of C_00860178 (0x006B2820, __thiscall, ret 0).
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* /*edx*/));

// Constructor for C_00860d90: runs the base constructor, installs this class's vtable,
// and zeroes three 32-bit fields at +4, +8, +0xC, then sets +0x10 to 10.
// Returns the object pointer (the original returns `this` in eax).
static C_00860d90* __fastcall C_00860d90_Construct(C_00860d90* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860D90);
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;

    return self;
}

HOOK(0x006CFBE0, C_00860d90_Construct, "auto");
