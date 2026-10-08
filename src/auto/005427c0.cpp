// Reimplementation of 0x005427C0 (module: menu_mp_character_select)
//
//   BOOL sub_5427C0()
//   {
//     if ( GetGlobal8B0688() && sub_551B70(GetGlobal8B0688()) )
//       return TRUE;
//     return FALSE;
//   }
//
// 0x00551B70 is a __thiscall (ecx = value returned by 0x00550CC0, no stack args,
// ret 0) returning a char flag.
#include <cstdint>
#include "hook.h"
#include "globals.h"

// int __cdecl sub_550CC0() -- ret 0
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// char __thiscall sub_551B70() -- ret 0
CSF_FUNC(char, __fastcall, MenuMpPause_GetMatchButtonState, 0x551B70, (void* self, void* edx));

static int __cdecl MenuMpCharacterSelect_IsActive()
{
    int result = 0;                                     // result = false
    if ( GetGlobal8B0688() )                            // call 0x550CC0; test eax,eax
    {
        // call 0x550CC0 again; mov ecx, eax; call 0x551B70
        if ( MenuMpPause_GetMatchButtonState(reinterpret_cast<void*>(GetGlobal8B0688()), nullptr) )
            return 1;                                   // mov eax, 1
    }
    return result;                                      // xor eax, eax
}

HOOK(0x5427C0, MenuMpCharacterSelect_IsActive, "auto");
