#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x004A8BC0 - base initializer invoked before the derived vtable pointers are
// installed. __thiscall (this=ecx) with one stack arg.
CSF_FUNC(uint32_t*, __fastcall, sub_4A8BC0, 0x004A8BC0,
         (void* self, void* edx, int a2));

// C_0083b3c0 constructor: runs the base initializer, installs the primary
// vtable (0x0083B3C0) and the secondary vtable (0x0083B3A4) at +0x04, clears
// the flag byte at +0x44, and returns this.
static C_0083b3c0* __fastcall C_0083b3c0_Construct(C_0083b3c0* self,
                                                   void* /*edx*/,
                                                   int a2)
{
    sub_4A8BC0(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083B3C0);                                  // primary vtable
    *reinterpret_cast<uint32_t*>(&self->field_0004) = CSF_ADDR_VT(0x0083B3A4); // secondary vtable
    self->field_0044 = 0;

    return self;
}

HOOK(0x4CF720, C_0083b3c0_Construct, "auto");
