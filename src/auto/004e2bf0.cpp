#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_4E1200 (__thiscall with two stack args, ret 8).
CSF_FUNC(uint32_t*, __fastcall, C_0083e160_InitBase, 0x004E1200, (void* self, void* edx, int a2, int a3));

// Constructor for C_0083e6d8: initializes the base part, zeroes the local
// state fields, then installs this class's primary vtable and the secondary
// vtable pointer stored in field_0004.
static int __fastcall C_0083e6d8_Construct(C_0083e6d8* self, void* /*edx*/, int a2, int a3)
{
    C_0083e160_InitBase(self, nullptr, a2, a3);

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    self->vtbl = CSF_VTBL(0x0083E6D8);
    self->field_0004 = (uint32_t)CSF_ADDR_VT(0x0083E6BC);

    return reinterpret_cast<int>(self);
}

HOOK(0x004E2BF0, C_0083e6d8_Construct, "auto");
