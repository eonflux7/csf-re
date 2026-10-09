#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_46BED0(int a1, int a2, int a3, uint32_t context, uint32_t flag)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(int, __cdecl, MissionScript_ParseEntry, 0x0046BED0,
         (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_46C410.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` saves ecx in a slot at
// [ebp-4]; a zero byte is then written over its low byte and the dword is
// reloaded, so the flag argument is ecx with its low byte cleared. The relayed
// call is:
//
//     MissionScript_ParseEntry(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// ret 0xC is produced by __fastcall callee cleanup of the three stack arguments.
static int __fastcall MissionScript_ForwardToParseEntry(void* self, void* /*edx*/,
                                                        int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return MissionScript_ParseEntry(a1, a2, a3, context, flag);
}

HOOK(0x0046C410, MissionScript_ForwardToParseEntry, "auto");
