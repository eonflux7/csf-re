#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

// Constructor-like initializer for C_00860894: zeroes the leading byte and the
// header fields, sets the three 10-valued capacity/limit slots, and initializes
// the three embedded sub-objects at +0x50, +0x64 and +0x78.
static C_00860894* __fastcall C_00860894_Construct(C_00860894* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x0) = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;

    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 10;

    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 10;

    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 10;

    C_00860894_Init((uint8_t*)self + 0x50, nullptr, -1, 0);
    C_00860894_Init((uint8_t*)self + 0x64, nullptr, -1, 0);
    C_00860894_Init((uint8_t*)self + 0x78, nullptr, -1, 0);

    return self;
}

HOOK(0x006CB070, C_00860894_Construct, "auto");
