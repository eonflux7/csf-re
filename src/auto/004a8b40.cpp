#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004A8BC0: base-class constructor chain for the C_00835140 portion of the object.
// Its result is ignored by the original; it pops its one stack argument (ret 4).
CSF_FUNC(uint32_t*, __fastcall, C_00835140_ConstructBase, 0x004A8BC0,
         (void* self, void* edx, int a2));

// C_00835060 constructor (0x004A8B40):
// runs the base constructor chain, installs C_00835060's vtable at +0,
// stores the secondary data pointer at +4, and returns the object pointer.
static uint32_t* __fastcall C_00835060_Construct(C_00835060* self, void* /*edx*/, int a2)
{
  C_00835140_ConstructBase(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_VTBL(0x00835060));
  CSF_AT(uint32_t, self, 4) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00835040));
  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4A8B40, C_00835060_Construct, "auto");
