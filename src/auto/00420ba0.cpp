#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00420BA0 (__thiscall, ret 0)
// Loads the pointer stored at this+0x8, stores 0 into a dword at that object's +0x1FC,
// and returns the loaded pointer value in eax.
static int __fastcall GetInnerObjectAndClearField1FC(void* self, void* /*edx*/)
{
  const uint32_t innerObject = CSF_AT(uint32_t, self, 0x8);
  *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(innerObject) + 0x1FC) = 0;
  return static_cast<int>(innerObject);
}

HOOK(0x420BA0, GetInnerObjectAndClearField1FC, "auto");
