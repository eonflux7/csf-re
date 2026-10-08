#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x007A8ED0: evaluates a condition on a2, result returned in eax (only al is tested here).
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x7A8ED0, (int a2));

// 0x0053A7A0 (__thiscall, ret 8)
// `self` (ecx) points 4 bytes into the real owner object; the owner lives at (self - 4).
// Evaluates the condition on a2 and, when the low byte of the result is non-zero,
// dispatches the owner's virtual slot 17 (byte offset 0x44, a zero-argument __thiscall),
// forwarding that call's return value; otherwise returns the raw condition value.
static int __fastcall CMenuMain_NotifyOwnerIfTrue(void* self, void* /*edx*/, int a2, int /*a3*/)
{
  int result = Sub_7A8ED0(a2);
  if ((uint8_t)result)
  {
    void* owner = reinterpret_cast<uint8_t*>(self) - 4;
    void** vtbl = *reinterpret_cast<void***>(owner);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[17]))(owner, nullptr);
  }
  return result;
}

HOOK(0x53A7A0, CMenuMain_NotifyOwnerIfTrue, "auto");
