#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00542850  bool sub_542850()  (rets=[0] -> __cdecl, no stack args)
CSF_FUNC(char, __cdecl, MenuMPCharacterSelect_QuerySelection, 0x542850, ());

// 0x00542A70  char __thiscall sub_542A70(std::uint8_t* this, char a2)   ret 4
static char __fastcall MenuMPCharacterSelect_UpdateEnabledState(uint8_t* self, void* /*edx*/, char a2)
{
    char result = a2;
    if (a2)
    {
        result = MenuMPCharacterSelect_QuerySelection();
        CSF_AT(uint8_t, self, 8) = (result == 0) ? 1 : 0;   // sete
        CSF_AT(uint8_t, self, 9) = (uint8_t)result;
    }
    else
    {
        CSF_AT(uint8_t, self, 8) = 0;
        CSF_AT(uint8_t, self, 9) = 0;
    }
    return result;
}

HOOK(0x542A70, MenuMPCharacterSelect_UpdateEnabledState, "auto");
