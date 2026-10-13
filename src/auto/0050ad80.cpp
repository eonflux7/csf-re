#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_695260, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteFloat, 0x00695330, (void* self, void* edx, float a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_QUALITY, 0x00899A5C);
CSF_GLOBAL(const char*, g_pszKey_MODE, 0x00899A60);
CSF_GLOBAL(const char*, g_pszKey_BRIGHTNESS, 0x00899A64);
CSF_GLOBAL(const char*, g_pszKey_NOISE, 0x00899A68);
CSF_GLOBAL(const char*, g_pszKey_VERTICAL_SYNC, 0x00899A6C);
CSF_GLOBAL(const char*, g_pszKey_ANTIALIAS, 0x00899A70);
CSF_GLOBAL(const char*, g_pszKey_SHADOWS, 0x00899A74);
CSF_GLOBAL(const char*, g_pszKey_TEX_FILTER, 0x00899A78);
CSF_GLOBAL(const char*, g_pszKey_TEX_QUALITY, 0x00899A7C);
CSF_GLOBAL(const char*, g_pszKey_MODEL_QUALITY, 0x00899A80);
CSF_GLOBAL(const char*, g_pszKey_EFFECTS_QUALITY, 0x00899A84);
CSF_GLOBAL(const char*, g_pszKey_WATER_QUALITY, 0x00899A88);
CSF_GLOBAL(const char*, g_pszKey_VIDEO, 0x00899A90);

static char __fastcall OptionsFile_WriteVideoSettings(
    void* self, void* /*edx*/, S_00694cd0* log)
{
    CoreZlibLog_BeginGroup(log, nullptr, const_cast<char*>(g_pszKey_VIDEO), 0, 0);
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x0C),
                            const_cast<char*>(g_pszKey_QUALITY));

    CoreZlibLog_BeginGroup(log, nullptr, const_cast<char*>(g_pszKey_MODE), 1, 1);
    CoreZlibLog_WriteFloat(log, nullptr, static_cast<float>(CSF_AT(int, self, 0x04)), nullptr);
    CoreZlibLog_WriteFloat(log, nullptr, static_cast<float>(CSF_AT(int, self, 0x08)), nullptr);
    CoreZlibLog_EndGroup_695260(log, nullptr);

    CoreZlibLog_WriteFloat(log, nullptr, CSF_AT(float, self, 0x10),
                          const_cast<char*>(g_pszKey_BRIGHTNESS));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(uint8_t, self, 0x15) != 0,
                            const_cast<char*>(g_pszKey_VERTICAL_SYNC));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x18),
                            const_cast<char*>(g_pszKey_ANTIALIAS));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x1C),
                            const_cast<char*>(g_pszKey_SHADOWS));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x20),
                            const_cast<char*>(g_pszKey_TEX_FILTER));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x24),
                            const_cast<char*>(g_pszKey_TEX_QUALITY));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x28),
                            const_cast<char*>(g_pszKey_MODEL_QUALITY));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x2C),
                            const_cast<char*>(g_pszKey_EFFECTS_QUALITY));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(int, self, 0x30),
                            const_cast<char*>(g_pszKey_WATER_QUALITY));
    CoreZlibLog_WriteInteger(log, nullptr, CSF_AT(uint8_t, self, 0x14) != 0,
                            const_cast<char*>(g_pszKey_NOISE));
    CoreZlibLog_EndGroup_6951D0(log, nullptr);
    return 1;
}

HOOK(0x0050AD80, OptionsFile_WriteVideoSettings, "auto");
