#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00561B90: base constructor that initializes the CMenuMpSessions / CMenuScreen
// portion of the object, forwards the construction argument and returns the object.
CSF_FUNC(CMenuMpSessionsPC*, __fastcall, CMenuMpSessions_Construct, 0x561B90,
         (CMenuMpSessionsPC* self, void* /*edx*/, int a2));

// CMenuMpSessionsPC constructor (0x00565FE0):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base vtable at +4, finally returning the object pointer.
static CMenuMpSessionsPC* __fastcall CMenuMpSessionsPC_Construct(
    CMenuMpSessionsPC* self, void* /*edx*/, int a2)
{
  CMenuMpSessions_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00845D48));
  self->field_0004 = CSF_ADDR_VT(0x00845CF8);
  return self;
}

HOOK(0x565FE0, CMenuMpSessionsPC_Construct, "auto");
