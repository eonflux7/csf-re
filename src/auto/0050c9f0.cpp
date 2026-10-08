#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_50C8E0(int a1, int a2, int a3, uint32_t context, uint32_t flag)
// The original pushes five arguments (0x14 stack bytes) even though the
// recovered prototype only reports the three leading ones; the callee is
// __cdecl, so the caller pops the full argument frame.
CSF_FUNC(int, __cdecl, MenuManager_Dispatch, 0x0050C8E0,
         (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_50C9F0.
//
// Entered with three stack arguments and consuming the incoming ecx register,
// which is modelled as the __thiscall-style "self" parameter (__fastcall keeps
// the original ecx access, the stack layout and the `ret 12` of the original).
// The prologue `push ecx` reserves a slot holding the incoming ecx; a single
// zero byte is then written over it (`mov byte ptr [ebp-4], 0`) and the whole
// dword reloaded, so only the low byte of ecx is cleared. The relayed call is:
//
//     MenuManager_Dispatch(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
static int __fastcall ForwardToMenuManagerDispatch(void* self, void* /*edx*/,
                                                   int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return MenuManager_Dispatch(a1, a2, a3, context, flag);
}

HOOK(0x0050C9F0, ForwardToMenuManagerDispatch, "auto");
