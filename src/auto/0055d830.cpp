#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x007A8ED0: condition helper whose result is tested via al; the full eax value is
// forwarded on the false path, so declare a 32-bit int return.
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x7A8ED0, (int a2));

// 0x0055D830
// `self` (ecx) points 4 bytes into the real owner object; the owner begins at (self - 4),
// its first dword being the vtable pointer. Evaluates the condition on a2 and, when true,
// dispatches the owner's virtual slot 19 (byte offset 0x4C) with no arguments, returning
// that call's result; otherwise returns the condition value itself.
static int __fastcall CMenuAccountEdit_NotifyOwnerIfTrue(void* self, void* /*edx*/, int a2, int a3)
{
  int result = Sub_7A8ED0(a2);
  if ((uint8_t)result)
  {
    void* owner = reinterpret_cast<uint8_t*>(self) - 4;
    void** vtbl = *reinterpret_cast<void***>(owner);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[19]))(owner, nullptr);
  }
  return result;
}

HOOK(0x55D830, CMenuAccountEdit_NotifyOwnerIfTrue, "auto");
