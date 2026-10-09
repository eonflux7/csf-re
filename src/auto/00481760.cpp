#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_480D00(int a1, int a2, int a3, uint32_t context, uint32_t flag)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(int, __cdecl, CharActionFsm_DispatchFiveArgs, 0x00480D00,
         (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_481760.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` reserves a slot holding
// ecx; a zero byte is written over its low byte and the dword is reloaded, so
// the flag argument is ecx with its low byte cleared. The relayed call is:
//
//     CharActionFsm_DispatchFiveArgs(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
static int __fastcall CharActionFsm_ForwardToDispatch(void* self, void* /*edx*/,
                                                      int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CharActionFsm_DispatchFiveArgs(a1, a2, a3, context, flag);
}

HOOK(0x00481760, CharActionFsm_ForwardToDispatch, "auto");
