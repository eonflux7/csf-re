#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x004D1660 - base task initializer (C_0083b958 / TCharTask). __thiscall (this=ecx), one stack arg.
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660,
         (C_0083b958* self, void* , int a2));

// C_0083c388 constructor: runs the base task initializer, installs the primary
// vtable (0x0083C388), stores the secondary vtable (0x0083C368) at +0x04, clears
// the flag byte at +0x40, and returns this.
static C_0083c388* __fastcall C_0083c388_Construct(C_0083c388* self,
                                                   void* /*edx*/,
                                                   int a2)
{
    AiAutonomous_ConstructTaskBase(reinterpret_cast<C_0083b958*>(self), nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083C388);                                   // primary vtable
    CSF_AT(uint32_t, self, 0x004) = CSF_ADDR_VT(0x0083C368);             // secondary vtable
    self->field_0040 = 0;

    return self;
}

HOOK(0x4D64B0, C_0083c388_Construct, "auto");
