#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_54E530(int a1, int a2, int a3, int a4, int a5)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(int, __cdecl, Relay54E530_Dispatch, 0x0054E530,
         (int a1, int a2, int a3, int a4, int a5));

// Reimplementation of sub_54E6D0.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` saves ecx in a slot at
// [ebp-4]; a zero byte is then written over its low byte and the dword is
// reloaded, so the fifth argument is ecx with its low byte cleared. The relayed
// call is:
//
//     Relay54E530_Dispatch(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// ret 0xC is produced by __fastcall callee cleanup of the three stack arguments.
static int __fastcall Forward3ArgsWithContextTo54E530(void* self, void* /*edx*/,
                                                      int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return Relay54E530_Dispatch(a1, a2, a3, static_cast<int>(context), static_cast<int>(flag));
}

HOOK(0x0054E6D0, Forward3ArgsWithContextTo54E530, "auto");
