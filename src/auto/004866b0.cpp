#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_484090(int a1, int a2, int a3, int a4, int a5)
// The original pushes five arguments (0x14 stack bytes); the callee is __cdecl,
// so the caller pops the full argument frame.
CSF_FUNC(int, __cdecl, CharActionFsm_DispatchWithContext, 0x00484090,
         (int a1, int a2, int a3, int a4, int a5));

// Reimplementation of sub_4866B0.
//
// Entered with the incoming ecx as `self` and three stack arguments. The
// prologue `push ecx` saves the incoming ecx in a local slot; a single zero byte
// is then written over its low byte (`mov byte ptr [ebp-4], 0`) and that dword
// is pushed as the flag argument. The relayed call is:
//
//     CharActionFsm_DispatchWithContext(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The stack arguments are pushed as a1 (first), a2, a3 at the call site; the
// callee receives them in that order, followed by the context and flag.
static int __fastcall ForwardToCharActionDispatch(void* self, void* /*edx*/,
                                                  int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CharActionFsm_DispatchWithContext(a1, a2, a3,
                                             static_cast<int>(context),
                                             static_cast<int>(flag));
}

HOOK(0x004866B0, ForwardToCharActionDispatch, "auto");
