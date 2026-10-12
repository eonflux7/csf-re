#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0041E1D0: base constructor for the C_0082e598 portion of the object.
// Its result is ignored by the original, and it pops its one stack argument (ret 4).
CSF_FUNC(uint32_t*, __fastcall, C_0082e598_Construct, 0x0041E1D0,
         (C_0082e598* self, void* , int initialValue));

// C_0082fbe8 constructor (0x00424DE0):
// runs the base constructor, installs C_0082fbe8's vtable at +0, stores the
// secondary base data pointer at +4, and returns the object pointer.
static uint32_t* __fastcall C_0082fbe8_ConstructWithInitialValue(
    C_0082fbe8* self, void* /*edx*/, int a2)
{
  C_0082e598_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_VTBL(0x0082FBE8));
  CSF_AT(uint32_t, self, 4) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082FBCC));
  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x424DE0, C_0082fbe8_ConstructWithInitialValue, "auto");
