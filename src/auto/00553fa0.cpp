#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_551D60(int a1, int a2, int a3, int a4, int a5)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(int, __cdecl, SpawnPoints_CopyWordRange, 0x00551D60,
         (int a1, int a2, int a3, int a4, int a5));

// Reimplementation of 0x553FA0.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` saves ecx at [ebp-4];
// its low byte is then cleared and the dword reloaded, so the flag argument is
// ecx with its low byte cleared. The relayed call is:
//
//     SpawnPoints_CopyWordRange(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// ret 0xC is produced by __fastcall callee cleanup of the three stack arguments.
static int __fastcall S_00553fa0_CopyWordRangeAndReturnEnd(void* self, void* /*edx*/,
                                                           int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return SpawnPoints_CopyWordRange(a1, a2, a3, static_cast<int>(context),
                                     static_cast<int>(flag));
}

HOOK(0x00553FA0, S_00553fa0_CopyWordRangeAndReturnEnd, "auto");
