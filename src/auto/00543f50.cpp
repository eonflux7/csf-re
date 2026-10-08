#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00543FC0: base constructor for CMenuGameOptionsPC; initializes the
// CMenuGameOptionsPC / CMenuScreen portion of the object and forwards the
// construction argument.
CSF_FUNC(int, __fastcall, CMenuGameOptionsPC_Construct, 0x543FC0,
         (CMenuGameOptionsInGamePC* self, void* /*edx*/, int a2));

// CMenuGameOptionsInGamePC constructor (0x00543F50):
// runs the base class constructor, then installs the derived class vtable at
// +0 and the secondary base vtable at +4, finally returning the object pointer.
static CMenuGameOptionsInGamePC* __fastcall CMenuGameOptionsInGamePC_Construct(
    CMenuGameOptionsInGamePC* self, void* /*edx*/, int a2)
{
  CMenuGameOptionsPC_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(0x008438F0);
  self->field_0004 = 0x008438A0;
  return self;
}

HOOK(0x543F50, CMenuGameOptionsInGamePC_Construct, "auto");
