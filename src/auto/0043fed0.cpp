#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __cdecl, CopyMenuPropertyText, 0x0043FBE0, (void* a1, void* String1, void* a3));
CSF_FUNC(int, __cdecl, Campaign_ReadScenarioDetails, 0x0043FC60, (void* a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(const char*, __cdecl, GetFileNameComponent, 0x00694A60, (const char* name));
CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszKey_ESCENARIO, 0x008996C8);

static char __stdcall Campaign_ReadAvailableScenario(S_00692b80* properties, int scenario)
{
    if (!properties)
        return 0;

    auto* scenarioBytes = reinterpret_cast<std::uint8_t*>(scenario);
    auto* scenarioName = reinterpret_cast<C_0082d410*>(scenarioBytes + 4);

    CopyMenuPropertyText(properties, const_cast<char*>(g_pszKey_ESCENARIO),
                         scenarioName);

    auto* globalState = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    if (!CSF_AT(std::uint8_t, globalState, 0xA5C))
    {
        Campaign_ReadScenarioDetails(properties, scenario);
        return 1;
    }

    const char* sourceName;
    if (CSF_AT(std::uint32_t, scenarioName, 0x18) < 0x10u)
        sourceName = reinterpret_cast<const char*>(&scenarioName->field_0004);
    else
        sourceName = reinterpret_cast<const char*>(scenarioName->field_0004);

    char path[512];
    std::strcpy(path, sourceName);
    Path_ReplaceExtension(path, 512,
                          const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));

    alignas(8) std::uint8_t mapNameBytes[0x1C];
    auto& mapName = *reinterpret_cast<C_0082d410*>(mapNameBytes);
    CSF_AT(std::uint32_t, &mapName, 0x18) = 15;
    CSF_AT(std::uint32_t, &mapName, 0x14) = 0;
    reinterpret_cast<char*>(&mapName.field_0004)[0] = 0;

    AssignStringRange_429B60(
        &mapName, nullptr, const_cast<char*>(CSF_STR(0x00830FE4, "Maps/")), 5);

    const char* fileName = GetFileNameComponent(path);
    Profiles_AppendStringRange(
        &mapName, 0, const_cast<char*>(fileName),
        static_cast<unsigned int>(std::strlen(fileName)));

    char* mapPath;
    if (CSF_AT(std::uint32_t, &mapName, 0x18) < 0x10u)
        mapPath = reinterpret_cast<char*>(&mapName.field_0004);
    else
        mapPath = reinterpret_cast<char*>(mapName.field_0004);

    char available = Campaign_CheckScenarioFile(mapPath);
    if (!available)
        available = Campaign_CheckScenarioFile(path);

    if (available)
        Campaign_ReadScenarioDetails(properties, scenario);

    if (CSF_AT(std::uint32_t, &mapName, 0x18) >= 0x10u)
    {
        if (mapName.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(mapName.field_0004));
    }

    return available;
}

HOOK(0x0043FED0, Campaign_ReadAvailableScenario, "auto");
