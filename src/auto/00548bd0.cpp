#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x005403A0: base constructor that initializes the CMenuMissionInfo /
// CMenuScreen portion of the object and forwards the construction argument.
CSF_FUNC(C_00843ed0*, __fastcall, CMenuMissionInfo_Construct, 0x5403A0,
         (C_00843ed0* self, void* /*edx*/, int a2));

// CMenuExtras constructor (0x00548BD0):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base vtable at +4, finally returning the object pointer.
static C_00843ed0* __fastcall CMenuExtras_Construct(
    C_00843ed0* self, void* /*edx*/, int a2)
{
  CMenuMissionInfo_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(0x00843ED0);
  self->field_0004 = 0x00843E80;
  return self;
}

HOOK(0x548BD0, CMenuExtras_Construct, "auto");
