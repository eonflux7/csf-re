#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_695260, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteFloat, 0x00695330, (void* self, void* edx, float a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_AUTO_AIM, 0x00899A24);
CSF_GLOBAL(const char*, g_pszKey_SUBTITLES, 0x00899A28);
CSF_GLOBAL(const char*, g_pszKey_INVERT_VIEW, 0x00899A2C);
CSF_GLOBAL(const char*, g_pszKey_SENSIBILIDAD, 0x00899A30);
CSF_GLOBAL(const char*, g_pszKey_GAME_VIBRATION, 0x00899A34);
CSF_GLOBAL(const char*, g_pszKey_VOICE_MUTED, 0x00899A38);
CSF_GLOBAL(const char*, g_pszKey_JUEGO, 0x00899A98);

static int __fastcall OptionsFile_WriteGameSettings(
    void* self, void* /*edx*/, S_00694cd0* log)
{
    CoreZlibLog_BeginGroup(log, nullptr,
        const_cast<char*>(g_pszKey_JUEGO), 0, 0);

    CoreZlibLog_WriteInteger(log, nullptr,
        CSF_AT(uint8_t, self, 0x4) != 0,
        const_cast<char*>(g_pszKey_AUTO_AIM));
    CoreZlibLog_WriteInteger(log, nullptr,
        CSF_AT(uint8_t, self, 0x5) != 0,
        const_cast<char*>(g_pszKey_SUBTITLES));
    CoreZlibLog_WriteInteger(log, nullptr,
        CSF_AT(uint8_t, self, 0x6) != 0,
        const_cast<char*>(g_pszKey_INVERT_VIEW));

    CoreZlibLog_BeginGroup(log, nullptr,
        const_cast<char*>(g_pszKey_SENSIBILIDAD), 1, 1);
    CoreZlibLog_WriteFloat(log, nullptr,
        CSF_AT(float, self, 0x8), nullptr);
    CoreZlibLog_WriteFloat(log, nullptr,
        CSF_AT(float, self, 0xC), nullptr);
    CoreZlibLog_EndGroup_695260(log, nullptr);

    CoreZlibLog_WriteInteger(log, nullptr,
        CSF_AT(uint8_t, self, 0x7) != 0,
        const_cast<char*>(g_pszKey_GAME_VIBRATION));
    CoreZlibLog_WriteInteger(log, nullptr,
        CSF_AT(uint8_t, self, 0x10) != 0,
        const_cast<char*>(g_pszKey_VOICE_MUTED));
    CoreZlibLog_EndGroup_6951D0(log, nullptr);
    return 1;
}

HOOK(0x0050B120, OptionsFile_WriteGameSettings, "auto");
