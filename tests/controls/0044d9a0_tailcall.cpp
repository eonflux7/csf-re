#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0044D9A0
// Tail-call through the vtable of the object referenced at this+0x1C.
//   mov ecx, [ecx+0x1C]   ; target = self->member_001C
//   mov eax, [ecx]        ; vtbl   = *target
//   jmp dword ptr [eax+0x1C] ; vtbl[7](target)   -> __thiscall, no stack args (ret 0)
static int __fastcall C_0044d9a0_InvokeMemberVirtual(void* self, void* /*edx*/)
{
  void* target = CSF_AT(void*, self, 0x1c);
  void** vtbl = CSF_AT(void**, target, 0);
  return ((int(__fastcall*)(void*, void*))vtbl[7])(target, nullptr);
}

HOOK(0x44D9A0, C_0044d9a0_InvokeMemberVirtual, "auto");
