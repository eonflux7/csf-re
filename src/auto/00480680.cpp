#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Calls the virtual method at vtable offset 0x11C (slot 71) with the argument 9.
// Disassembly:
//   mov eax, [ecx]
//   push 9
//   call [eax + 0x11C]
//   ret
static int __fastcall InvokeVtblSlot71_Arg9(CCharWeaponFsm* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*, int))self->vtbl[0x11c / 4])(self, nullptr, 9);
}

HOOK(0x00480680, InvokeVtblSlot71_Arg9, "auto");
