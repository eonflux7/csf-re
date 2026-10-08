#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x007A8ED0: returns a condition flag in al (full eax preserved by the caller of this target).
// Prototype in the packet is unknown; only its (int) -> int usage is observable here.
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x7A8ED0, (int a2));

// 0x0052F920
// `this` (ecx) points 4 bytes into the real owner object; the owner lives at (this - 4).
// Evaluates the condition on a2 and, when true, dispatches the owner's virtual slot 16
// (byte offset 0x40), forwarding that call's return value; otherwise returns the condition value.
static int __fastcall CMenuScenarioSelect_NotifyOwnerIfTrue(void* self, void* /*edx*/, int a2, int a3)
{
  int result = Sub_7A8ED0(a2);
  if ((uint8_t)result)
  {
    void* owner = reinterpret_cast<uint8_t*>(self) - 4;
    void** vtbl = *reinterpret_cast<void***>(owner);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[16]))(owner, nullptr);
  }
  return result;
}

HOOK(0x52F920, CMenuScenarioSelect_NotifyOwnerIfTrue, "auto");
