#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor (TCharTask chain), bound by address.
CSF_FUNC(uint32_t*, __fastcall, C_0082e598_Construct, 0x0041E1D0, (C_0082e598* self, void* , int initialValue));

// Constructor for C_0082fdb8: runs the base constructor, then initializes
// its own fields and vtables. Returns this (__thiscall, ret 4).
static int __fastcall C_0082fdb8_Construct(C_0082fdb8* self, void* /*edx*/, int a2)
{
    C_0082e598_Construct(reinterpret_cast<C_0082e598*>(self), nullptr, a2);
    self->field_0028 = 0;
    self->field_0020 = 0;
    self->vtbl = CSF_VTBL(0x0082FDB8);
    CSF_AT(uint32_t, self, 0x04) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082FD9C));
    self->field_0024 = 1;
    return reinterpret_cast<int>(self);
}

HOOK(0x425560, C_0082fdb8_Construct, "auto");
