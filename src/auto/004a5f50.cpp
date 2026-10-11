#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004A5760: base constructor for the C_00834598 portion of the object.
// Its result is ignored by the original, and it pops its one stack argument (ret 4).
CSF_FUNC(int, __fastcall, C_00834598_InitializeForSceneObject, 0x004A5760,
         (void* self, void* edx, int a2));

// C_00834640 constructor (0x004A5F50):
// runs the base constructor, installs C_00834640's vtable at +0, stores the
// secondary base data pointer at +4, and returns the object pointer.
static uint32_t* __fastcall C_00834640_ConstructForCharacter(
    C_00834640* self, void* /*edx*/, int a2)
{
  C_00834598_InitializeForSceneObject(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_VTBL(0x00834640));
  self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834620));
  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4A5F50, C_00834640_ConstructForCharacter, "auto");
