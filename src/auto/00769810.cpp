#include <cstdint>
#include "globals.h"
#include "hook.h"

// uint32_t* __cdecl sub_768F30(uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5)
// Five stack arguments are pushed (0x14 bytes), so the cdecl caller pops them.
CSF_FUNC(uint32_t*, __cdecl, InputBindings_CreateActionEntry, 0x00768F30,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// Reimplementation of 0x00769810.
//
// Entered with three stack arguments and the incoming ecx, modelled as the
// __thiscall "self" parameter. The prologue `push ecx` saves ecx in a slot at
// [ebp-4]; a zero byte is then written over its low byte and the dword is
// reloaded, so the flag argument is ecx with its low byte cleared. The relayed
// call is:
//
//     InputBindings_CreateActionEntry(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// ret 0xC is produced by __fastcall callee cleanup of the three stack arguments.
static uint32_t* __fastcall InputBindings_ForwardToCreateActionEntry(void* self, void* /*edx*/,
                                                                     uint32_t* a1, uint32_t* a2,
                                                                     uint32_t* a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return InputBindings_CreateActionEntry(a1, a2, a3, static_cast<int>(context),
                                           static_cast<int>(flag));
}

HOOK(0x00769810, InputBindings_ForwardToCreateActionEntry, "auto");
