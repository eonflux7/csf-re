#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004D1660: base constructor for the TCharTask portion of the object.
// Its result is ignored by the original, and it pops its one stack argument (ret 4).
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660,
         (void* self, void* edx, int a2));

// C_0083b038 constructor (0x004CEEE0):
// runs the base constructor, installs C_0083b038's vtable at +0, stores the
// secondary base data pointer at +4, and returns the object pointer.
static uint32_t* __fastcall C_0083b038_ConstructAiAutonomousTask(
    C_0083b038* self, void* /*edx*/, int a2)
{
  AiAutonomous_ConstructTaskBase(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_VTBL(0x0083B038));
  CSF_AT(uint32_t, self, 4) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0083B018));
  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4CEEE0, C_0083b038_ConstructAiAutonomousTask, "auto");
