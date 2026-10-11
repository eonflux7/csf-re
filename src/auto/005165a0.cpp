#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ProfilesSavegames_CopyScenarioRecords, 0x00515970,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, ProfilesSavegames_InsertScenarioRecords, 0x005160B0,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40,
         (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910,
         (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_ESCENARIO_ID_profiles_savegames, 0x00899BD8);
CSF_GLOBAL(const char*, g_pszKey_PUNTUACION, 0x00899BDC);
CSF_GLOBAL(const char*, g_pszKey_MISION_EXITO, 0x00899BE0);
CSF_GLOBAL(const char*, g_pszKey_NUM_ESTRELLAS, 0x00899BE4);

static const auto kComponentVtable = CSF_VTBL(0x00692B80);

static char __stdcall ProfilesSavegames_LoadScenarioResults(
    S_00692b80* root, int profileAddress)
{
    if (!root)
        return 0;

    alignas(8) std::uint8_t recordBytes[0x14]{};
    CSF_AT(std::uint32_t, recordBytes, 0x10) =
        MenuExtras_GetChildCount(root, nullptr);

    auto* profile = reinterpret_cast<std::uint8_t*>(profileAddress);
    void* records = profile + 0x34;

    if (CSF_AT(std::uint32_t, profile, 0x38))
        Mem_FreeDebug(reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, profile, 0x38)));

    CSF_AT(std::uint32_t, profile, 0x38) = 0;
    CSF_AT(std::uint32_t, profile, 0x3C) = 0;
    CSF_AT(std::uint32_t, profile, 0x40) = 0;
    CSF_AT(int, recordBytes, 0x00) = -1;
    CSF_AT(int, recordBytes, 0x04) = 0;
    CSF_AT(std::uint8_t, recordBytes, 0x08) = 0;
    CSF_AT(int, recordBytes, 0x0C) = 0;

    if (CSF_AT(std::uint32_t, recordBytes, 0x10) != 0)
    {
        std::uint32_t index = 0;
        do
        {
            auto* component = reinterpret_cast<std::uint8_t*>(
                GetComponent(reinterpret_cast<unsigned char*>(root), nullptr, index));

            CSF_AT(int, recordBytes, 0x00) = ReadIntegerProperty(
                component, nullptr,
                const_cast<char*>(g_pszKey_ESCENARIO_ID_profiles_savegames));
            CSF_AT(int, recordBytes, 0x04) = ReadIntegerProperty(
                component, nullptr, const_cast<char*>(g_pszKey_PUNTUACION));

            auto* property = FindProperty(
                component, nullptr, const_cast<char*>(g_pszKey_MISION_EXITO));
            if (property)
                CSF_AT(std::uint8_t, recordBytes, 0x08) =
                    ParseIntegerComponent(
                        reinterpret_cast<std::uint32_t>(property)) == 1;

            property = FindProperty(
                component, nullptr, const_cast<char*>(g_pszKey_NUM_ESTRELLAS));
            if (property)
                CSF_AT(int, recordBytes, 0x0C) = ParseIntegerComponent(
                    reinterpret_cast<std::uint32_t>(property));

            const std::uint32_t begin = CSF_AT(std::uint32_t, profile, 0x38);
            const std::uint32_t size = begin
                ? static_cast<std::uint32_t>(
                      static_cast<std::int32_t>(
                          CSF_AT(std::uint32_t, profile, 0x3C) - begin) >> 4)
                : 0;

            if (begin &&
                size < static_cast<std::uint32_t>(
                           static_cast<std::int32_t>(
                               CSF_AT(std::uint32_t, profile, 0x40) - begin) >> 4))
            {
                const std::uint32_t end = CSF_AT(std::uint32_t, profile, 0x3C);
                ProfilesSavegames_CopyScenarioRecords(
                    static_cast<int>(end), 1,
                    reinterpret_cast<int>(recordBytes),
                    reinterpret_cast<int>(records),
                    reinterpret_cast<int>(root));
                CSF_AT(std::uint32_t, profile, 0x3C) = end + 16;
            }
            else
            {
                ProfilesSavegames_InsertScenarioRecords(
                    records, nullptr,
                    reinterpret_cast<char*>(CSF_AT(std::uint32_t, profile, 0x3C)),
                    1u, reinterpret_cast<std::uint32_t*>(recordBytes));
            }

            ++index;
        } while (index < CSF_AT(std::uint32_t, recordBytes, 0x10));
    }

    return 1;
}

HOOK(0x005165A0, ProfilesSavegames_LoadScenarioResults, "auto");
