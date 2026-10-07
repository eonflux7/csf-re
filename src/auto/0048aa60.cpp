#include <cstdint>
#include "hook.h"
#include "globals.h"

// Original callee at 0x005189D0: int sub_5189D0()  (ret 0 -> __cdecl, no args)
CSF_FUNC(int, __cdecl, Get_Global_8B0578, 0x5189D0, ());

// Thunk at 0x0048AA60. Hex-Rays prototype:
//   int __stdcall sub_48AA60(int a1, int a2, int a3)
// It discards its three stack arguments and simply forwards to
// Get_Global_8B0578 (0x005189D0), returning its result unchanged.
// __stdcall with 3 args => the callee pops 12 bytes (ret 0xc), matching the original.
static int __stdcall GetGlobal8B0578Thunk(int /*arg0*/, int /*arg1*/, int /*arg2*/)
{
    return Get_Global_8B0578();
}

HOOK(0x48AA60, GetGlobal8B0578Thunk, "auto");
