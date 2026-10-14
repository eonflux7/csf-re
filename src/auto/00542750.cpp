#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Localized menu strings used by the multiplayer character-select screen.
CSF_GLOBAL(const char*, g_pszMenu_P_LbLaWa, 0x00899B74);
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);

// 0x00542750 (__thiscall, ret 4):
// Picks between the two localized label strings depending on a flag byte at
// this+0xA, then forwards the chosen string to a virtual method (vtable slot
// 0x58/4) of the object passed in and returns its result.
static int __fastcall MenuMpCharacterSelect_SetStatusLabel(std::uint8_t* self, void* /*edx*/, void* target)
{
    const char* text = g_pszMenu_P_LbLaWa;
    if (!CSF_AT(uint8_t, self, 0xA))
        text = g_pszMenu_P_LbMePr;

    return reinterpret_cast<int(__thiscall*)(void*, const char*)>(CSF_AT(void**, target, 0)[0x58 / 4])(target, text);
}

HOOK(0x00542750, MenuMpCharacterSelect_SetStatusLabel, "auto");
