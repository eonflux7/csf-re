#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x0053A940 - base constructor/initializer invoked before the derived
// vtable pointers are installed.  __thiscall (this=ecx) with one stack arg.
CSF_FUNC(void, __fastcall, CMenuMain_Construct, 0x53A940,
         (CMenuMainPC* self, void* /*edx*/, int a2));

// CMenuMainPC constructor: runs the base initializer, installs the
// CMenuMainPC primary vtable (0x00843830) and the secondary vtable
// (0x008437E0) at +0x04, sets the flag byte at +0x3C, returns this.
static CMenuMainPC* __fastcall CMenuMainPC_Construct(CMenuMainPC* self,
                                                     void* /*edx*/,
                                                     int a2)
{
    CMenuMain_Construct(self, nullptr, a2);

    *reinterpret_cast<uint32_t*>(self) = 0x00843830u; // CMenuMainPC vtable
    self->field_0004 = 0x008437E0u;                   // secondary vtable
    self->field_003c = 1;

    return self;
}

HOOK(0x543C40, CMenuMainPC_Construct, "auto");
