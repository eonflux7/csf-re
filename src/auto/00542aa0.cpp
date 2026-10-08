#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00543150: initializes the CMenuControls base portion of the object and
// forwards the construction argument. __thiscall with one stack arg (ret 4).
CSF_FUNC(CMenuControlsInGame*, __fastcall, CMenuControls_Construct, 0x543150,
         (CMenuControlsInGame* self, void* /*edx*/, int a2));

// CMenuControlsInGame constructor (0x00542AA0):
// runs the CMenuControls base constructor, then installs the derived class
// vtable at +0 and the secondary base vtable at +4, finally returning the
// object pointer.
static CMenuControlsInGame* __fastcall CMenuControlsInGame_Construct(
    CMenuControlsInGame* self, void* /*edx*/, int a2)
{
  CMenuControls_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00843550));
  self->field_0004 = 0x00843500;
  return self;
}

HOOK(0x542AA0, CMenuControlsInGame_Construct, "auto");

