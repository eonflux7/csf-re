#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-chain initializer that this constructor calls first (4 stack args, ret 16).
CSF_FUNC(float*, __fastcall, C0083ec48_InitBaseChain, 0x004E64F0,
         (void* self, void* edx, int a2, float a3, int a4, int a5));

// Constructor for C_0083ec48: runs the base-chain init, then installs this class's
// vtable and the secondary vtable pointer at +0x4.
static C_0083ec48* __fastcall C_0083ec48_Construct(C_0083ec48* self, void* /*edx*/,
                                                   int a2, float a3, int a4, int a5)
{
    C0083ec48_InitBaseChain(self, nullptr, a2, a3, a4, a5);
    self->vtbl = CSF_VTBL(0x0083EC48);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083EC2C);
    return self;
}

HOOK(0x004E8AA0, C_0083ec48_Construct, "auto");
