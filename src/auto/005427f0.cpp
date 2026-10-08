// Reimplementation of 0x005427F0 (module: menu_mp_character_select)
//
//   BOOL sub_5427F0()
//   {
//     BOOL result = FALSE;
//     if ( GetGlobal8B0688() )
//     {
//       int v0 = GetGlobal8B0688();
//       if ( (unsigned __int8)sub_551BF0(v0) )
//         return TRUE;
//     }
//     return result;
//   }
//
// 0x00551BF0 is a __thiscall (ecx = value returned by 0x00550CC0, no stack args,
// ret 0) whose byte result is tested (test al, al) -- a clone of 0x00551B70 used by
// the same-shape function 0x005427C0.
#include <cstdint>
#include "hook.h"
#include "globals.h"

// int __cdecl sub_550CC0() -- ret 0
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// char __thiscall sub_551BF0() -- ret 0
CSF_FUNC(char, __fastcall, MenuObject_CheckSecondaryState, 0x551BF0, (void* self, void* edx));

static int __cdecl MenuMpCharacterSelect_IsSecondaryStateActive()
{
    int result = 0;                                     // result = false
    if ( GetGlobal8B0688() )                            // call 0x550CC0; test eax,eax; je
    {
        // call 0x550CC0 again; mov ecx, eax; call 0x551BF0; test al, al
        if ( MenuObject_CheckSecondaryState(reinterpret_cast<void*>(GetGlobal8B0688()), nullptr) )
            return 1;                                   // mov eax, 1
    }
    return result;                                      // xor eax, eax
}

HOOK(0x5427F0, MenuMpCharacterSelect_IsSecondaryStateActive, "auto");
