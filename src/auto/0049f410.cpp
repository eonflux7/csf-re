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

// Base-class constructor C_00833908 (__thiscall, 2 stack args, ret 8).
CSF_FUNC(std::uint32_t*, __fastcall, C_00833908_Construct, 0x00490940,
         (C_00833908* self, void* /*edx*/, int objectId, int stateCode));

// C_008342e8 constructor: base init, then sets its vtable and sub-table pointers,
// zeroes the state fields and sets field_01a4 to 10. Returns this.
static C_008342e8* __fastcall C_008342e8_Construct(C_008342e8* self, void* /*edx*/, int a2, int a3)
{
    C_00833908_Construct(self, nullptr, a2, a3);

    self->vtbl = CSF_VTBL(0x008342E8);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00834268);
    CSF_AT(uint32_t, self, 0x08) = (uint32_t)CSF_ADDR_DATA(0x00834260);
    self->field_000c = (uint32_t)CSF_ADDR_DATA(0x00834254);
    CSF_AT(uint32_t, self, 0x10) = (uint32_t)CSF_ADDR_VT(0x0083424C);
    self->field_0014 = (uint32_t)CSF_ADDR_DATA(0x00834248);

    self->field_0198 = 0;
    self->field_019c = 0;
    self->field_01a0 = 0;
    self->field_01a4 = 10;
    self->field_01a8 = 0;
    self->field_01ac = 0;

    return self;
}

HOOK(0x0049F410, C_008342e8_Construct, "auto");
