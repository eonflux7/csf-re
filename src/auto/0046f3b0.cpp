#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_46EE50(int a1, int a2, int a3, uint32_t context, uint32_t flag)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(int, __cdecl, MissionScript_ParseEntryFiveArgs, 0x0046EE50,
         (int a1, int a2, int a3, int a4, int a5));

// Reimplementation of sub_46F3B0.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` reserves a slot holding
// ecx; a zero byte is written over its low byte and the dword is reloaded, so
// the flag argument is ecx with its low byte cleared. The relayed call is:
//
//     MissionScript_ParseEntryFiveArgs(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The original pops 12 bytes (ret 0xC), which the __fastcall wrapper with three
// stack parameters reproduces.
static int __fastcall MissionScript_ForwardToParser(void* self, void* /*edx*/,
                                                    int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return MissionScript_ParseEntryFiveArgs(a1, a2, a3, static_cast<int>(context),
                                            static_cast<int>(flag));
}

HOOK(0x0046F3B0, MissionScript_ForwardToParser, "auto");
