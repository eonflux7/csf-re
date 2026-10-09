#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(void, __cdecl, sub_42DF00, 0x0042DF00, (void* a1, int* a2));
CSF_GLOBAL(char*, g_pszKey_ICON, 0x008995CC);

static char __cdecl Hud_TryReadIconProperty(std::uint8_t* hudObject, int* iconOut, int* extraOut)
{
    if (hudObject == nullptr || FindProperty(hudObject, nullptr, g_pszKey_ICON) == nullptr)
        return 0;

    *iconOut = ReadIntegerProperty(hudObject, nullptr, g_pszKey_ICON);
    sub_42DF00(hudObject, extraOut);
    return 1;
}

HOOK(0x42E640, Hud_TryReadIconProperty, "auto");
