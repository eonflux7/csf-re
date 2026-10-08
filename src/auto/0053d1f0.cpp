#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0053CFC0: base constructor that builds the CMenuSoundOptions / CMenuScreen
// portion of the object and forwards the construction argument.
CSF_FUNC(CMenuSoundOptionsInGame*, __fastcall, CMenuSoundOptions_Construct, 0x53CFC0,
         (CMenuSoundOptionsInGame* self, void* /*edx*/, int a2));

// CMenuSoundOptionsInGame constructor (0x0053D1F0):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base vtable at +4, finally returning the object pointer.
static CMenuSoundOptionsInGame* __fastcall CMenuSoundOptionsInGame_Construct(
    CMenuSoundOptionsInGame* self, void* /*edx*/, int a2)
{
  CMenuSoundOptions_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00842BC8));
  self->field_0004 = 0x00842B78;
  return self;
}

HOOK(0x53D1F0, CMenuSoundOptionsInGame_Construct, "auto");
