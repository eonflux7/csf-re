#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Target: 0x00482830 (13 bytes, ret 0)
//
// int __thiscall sub_482830(void* this)
// {
//   return (*(int (__thiscall**)(void*, _DWORD, _DWORD))(*(_DWORD*)this + 308))(this, 0, 0);
// }
//
// Disassembly:
//   00482830  mov eax, dword ptr [ecx]      ; vtable
//   00482832  push 0
//   00482834  push 0
//   00482836  call dword ptr [eax + 0x134]  ; slot 77, __thiscall (this still in ecx)
//   0048283c  ret
//
// Forwards to a virtual method (vtable byte offset 0x134 = slot 77) with two null
// arguments and returns its 32-bit result untouched.
static int __fastcall InvokeVtbl134WithZeroArgs(void* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    return (reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(vtbl[0x134 / 4]))(self, nullptr, 0, 0);
}

HOOK(0x00482830, InvokeVtbl134WithZeroArgs, "auto");
