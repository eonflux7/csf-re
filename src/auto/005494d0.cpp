#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00549F00: base constructor that initializes the CMenuVideoOptions
// portion of the object and forwards the construction argument.
CSF_FUNC(CMenuVideoOptions*, __fastcall, CMenuVideoOptions_Construct, 0x549F00,
         (CMenuVideoOptions* self, void* /*edx*/, int a2));

// CMenuVideoOptionsInGame constructor (0x005494D0):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base vtable at +4, finally returning the object pointer.
static CMenuVideoOptionsInGame* __fastcall CMenuVideoOptionsInGame_Construct(
    CMenuVideoOptionsInGame* self, void* /*edx*/, int a2)
{
  CMenuVideoOptions_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x008440A0));
  self->field_0004 = CSF_ADDR_VT(0x00844050);
  return self;
}

HOOK(0x5494D0, CMenuVideoOptionsInGame_Construct, "auto");
