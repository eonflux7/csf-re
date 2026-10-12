#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of C_00860178, bound by address.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Constructor of C_00860c88 (SPHERE/ELLIPSOID shape): runs the base constructor,
// installs this class's vtable, zeroes two counters/handles and sets the last field to -1.
static uint32_t* __fastcall C_00860c88_Construct(C_00860c88* self, void* /*edx*/) {
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00860C88);
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = -1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006CECD0, C_00860c88_Construct, "auto");
