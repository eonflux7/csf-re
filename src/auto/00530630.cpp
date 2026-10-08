#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original function at 0x00530630 (size 35 bytes)
// Prototype: int __thiscall sub_530630(int *this)
// It calls GetGlobalValue (0x521BB0), then calls sub_5225D0 on the result,
// then if the member pointer at offset 0x1E8 is non-null, it tail-jumps
// to a virtual method at vtable slot 0x50/4 = 20 of that object.

CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(int, __fastcall, sub_5225D0, 0x5225D0, (void* self, void* edx));

// Naked thunk performing the indirect tail jump.
// The object pointer is passed in ecx (first __fastcall argument).
// It loads the vtable and jumps to slot 0x50, forwarding the caller's
// return address and stack unchanged.
static __declspec(naked) int CMenuMpServerInfo_TailCallMemberVirtual(int* obj, void* /*edx*/)
{
    __asm {
        mov eax, [ecx]
        jmp dword ptr [eax + 0x50]
    }
}

static int __fastcall CMenuMpServerInfo_InvokeMemberAction(int* self, void* /*edx*/)
{
    int* global = (int*)GetGlobalValue();
    sub_5225D0(global, nullptr);

    int* member = CSF_AT(int*, self, 0x1e8);
    if (member)
        return CMenuMpServerInfo_TailCallMemberVirtual(member, nullptr);
    return 0;
}

HOOK(0x530630, CMenuMpServerInfo_InvokeMemberAction, "auto");
