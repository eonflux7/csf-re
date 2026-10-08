#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0053AF80: base constructor that initializes the CMenuOptions / CMenuScreen
// portion of the object and forwards the construction argument.
CSF_FUNC(CMenuOptions*, __fastcall, CMenuOptions_Construct, 0x53AF80,
         (CMenuOptions* self, void* /*edx*/, int a2));

// CMenuOptionsPC constructor (0x0053B820):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base (CMenuOptions) vtable at +4, finally returning the object.
static CMenuOptionsPC* __fastcall CMenuOptionsPC_Construct(
    CMenuOptionsPC* self, void* /*edx*/, int a2)
{
  CMenuOptions_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00842858));
  self->field_0004 = 0x00842808;
  return self;
}

HOOK(0x53B820, CMenuOptionsPC_Construct, "auto");

