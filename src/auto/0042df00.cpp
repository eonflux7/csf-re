#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_WIDTH, 0x008995B4);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT, 0x008995B8);

// Fills an int[4] rect {left, top, right, bottom} from integer properties of a HUD widget.
static void __cdecl Hud_ReadRectFromProperties(S_00692b80* a1, int* a2)
{
    if (a1)
    {
        a2[1] = ReadIntegerProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr, const_cast<char*>(g_pszKey_TOP));
        a2[0] = ReadIntegerProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr, const_cast<char*>(g_pszKey_LEFT));
        a2[2] = ReadIntegerProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr, const_cast<char*>(g_pszKey_WIDTH)) + a2[0];
        a2[3] = ReadIntegerProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr, const_cast<char*>(g_pszKey_HEIGHT)) + a2[1];
    }
}

HOOK(0x42DF00, Hud_ReadRectFromProperties, "auto");
