#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0,
         (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndRecord, 0x006951D0,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup, 0x00695260,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0,
         (void* self, void* edx, int a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470,
         (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_CAMPA, 0x00899BD4);
CSF_GLOBAL(const char*, g_pszKey_ESCENARIO_ID_profiles_savegames, 0x00899BD8);
CSF_GLOBAL(const char*, g_pszKey_PUNTUACION, 0x00899BDC);
CSF_GLOBAL(const char*, g_pszKey_MISION_EXITO, 0x00899BE0);
CSF_GLOBAL(const char*, g_pszKey_NUM_ESTRELLAS, 0x00899BE4);

static int __stdcall ProfilesSavegames_WriteCampaignResults(
    S_00694cd0* log, void* profile)
{
    CoreZlibLog_BeginGroup(
        log, nullptr, const_cast<char*>(g_pszKey_CAMPA), 1, 0);

    const std::uint32_t begin = CSF_AT(std::uint32_t, profile, 0x38);
    std::uint32_t remaining = 0;
    if (begin)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, profile, 0x3C) - begin;
        remaining = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteCount) >> 4);
    }

    std::uint32_t byteOffset = 0;
    while (remaining != 0)
    {
        CoreZlibLog_BeginRecord(log, nullptr, 0, 0, 0);

        CoreZlibLog_WriteInteger(
            log, nullptr,
            CSF_AT(int, CSF_AT(std::uint8_t*, profile, 0x38) + byteOffset, 0),
            const_cast<char*>(g_pszKey_ESCENARIO_ID_profiles_savegames));

        CoreZlibLog_WriteInteger(
            log, nullptr,
            CSF_AT(int, CSF_AT(std::uint8_t*, profile, 0x38) + byteOffset, 4),
            const_cast<char*>(g_pszKey_PUNTUACION));

        CoreZlibLog_WriteInteger(
            log, nullptr,
            CSF_AT(std::uint8_t,
                   CSF_AT(std::uint8_t*, profile, 0x38) + byteOffset, 8) != 0,
            const_cast<char*>(g_pszKey_MISION_EXITO));

        CoreZlibLog_WriteInteger(
            log, nullptr,
            CSF_AT(int, CSF_AT(std::uint8_t*, profile, 0x38) + byteOffset, 12),
            const_cast<char*>(g_pszKey_NUM_ESTRELLAS));

        CoreZlibLog_EndRecord(log, nullptr);
        byteOffset += 16;
        --remaining;
    }

    return static_cast<int>(CoreZlibLog_EndGroup(log, nullptr));
}

HOOK(0x00515AF0, ProfilesSavegames_WriteCampaignResults, "auto");
