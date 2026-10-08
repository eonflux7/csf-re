#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original callee: int __thiscall sub_6E85F0(_DWORD* this, int a2)  -- ret 4
CSF_FUNC(int, __fastcall, C_00861e98_StoreValue54, 0x6E85F0, (uint32_t* self, void* edx, int a2));

// 0x0049EBF0:
//   push ebp / mov ebp, esp
//   mov ecx, [ecx+0x30]      <- re-seat ecx to the object pointer stored at self+0x30
//   pop ebp
//   jmp 0x6E85F0             <- tail-call, forwarding the single stack argument
// The tail-jumped callee is __thiscall and pops 4 bytes, so this wrapper must also
// consume exactly one 4-byte stack argument.
static int __fastcall ForwardToSubObjectAt30(void* self, void* /*edx*/, int a1)
{
    uint32_t* subObject = CSF_AT(uint32_t*, self, 0x30);
    return C_00861e98_StoreValue54(subObject, nullptr, a1);
}

HOOK(0x49EBF0, ForwardToSubObjectAt30, "auto");
