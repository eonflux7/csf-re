#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004A4280: base constructor for the object-controller portion. Takes the
// construction argument on the stack and pops it (ret 4).
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_ConstructObjectController, 0x004A4280,
         (void* self, void* edx, int a2));

// C_00834790 constructor (0x004A6C80):
// runs the base constructor with the argument, then installs the derived
// vtable at +0 and the secondary vtable at +4, and returns the object pointer.
static C_00834790* __fastcall C_00834790_ConstructForSceneObject(
    C_00834790* self, void* /*edx*/, int a2)
{
  SceneMissionSetup_ConstructObjectController(self, nullptr, a2);
  self->vtbl = CSF_VTBL(0x00834790);
  self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00834770));
  return self;
}

HOOK(0x004A6C80, C_00834790_ConstructForSceneObject, "auto");
