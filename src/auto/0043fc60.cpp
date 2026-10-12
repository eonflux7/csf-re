#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Hud_CreateTextureWidget, 0x0042D750, (S_00692b80* properties, int layer));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(void*, __cdecl, Campaign_CopyPropertyText, 0x0043FBE0, (void* a1, void* String1, void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszKey_ESCENARIO_ID, 0x008996CC);
CSF_GLOBAL(const char*, g_pszKey_ESCENARIO_INICIO, 0x008996D0);
CSF_GLOBAL(const char*, g_pszKey_SCREENSHOT, 0x008996D4);
CSF_GLOBAL(const char*, g_pszKey_PANTALLA_CARGA, 0x008996D8);
CSF_GLOBAL(const char*, g_pszKey_LAST_MISSION, 0x008996DC);
CSF_GLOBAL(const char*, g_pszKey_DESCRIPCION, 0x008996E0);
CSF_GLOBAL(const char*, g_pszKey_BRIEFING_CAMPAGNA, 0x008996E4);
CSF_GLOBAL(const char*, g_pszKey_BRIEFING_CARGA, 0x008996E8);
CSF_GLOBAL(const char*, g_pszKey_FICHERO_DESCRIPCION, 0x008996EC);
CSF_GLOBAL(const char*, g_pszKey_FICHERO_BRIEFING_CAMPAGNA, 0x008996F0);
CSF_GLOBAL(const char*, g_pszKey_FICHERO_BRIEFING_CARGA, 0x008996F4);

static void Campaign_ResetTemporaryString(C_0082d410& text)
{
    CSF_AT(unsigned int, &text, 0x18) = 15;
    CSF_AT(unsigned int, &text, 0x14) = 0;
    CSF_AT(char, &text, 0x04) = 0;
}

static const char* Campaign_GetTemporaryStringText(C_0082d410& text)
{
    if (CSF_AT(unsigned int, &text, 0x18) < 0x10)
        return reinterpret_cast<const char*>(&text.field_0004);
    return reinterpret_cast<const char*>(text.field_0004);
}

static int __cdecl Campaign_LoadScenarioProperties(S_00692b80* properties, int destination)
{
    auto* scenario = reinterpret_cast<std::uint8_t*>(destination);

    S_00692b80* screenshot = reinterpret_cast<S_00692b80*>(
        FindProperty(reinterpret_cast<std::uint8_t*>(properties), nullptr,
                     const_cast<char*>(g_pszKey_SCREENSHOT)));
    CSF_AT(int, scenario, 0xB8) = Hud_CreateTextureWidget(screenshot, 0);

    alignas(8) std::uint8_t descriptionBytes[0x1C];
    C_0082d410& description =
        *reinterpret_cast<C_0082d410*>(descriptionBytes);
    Campaign_ResetTemporaryString(description);

    Campaign_CopyPropertyText(properties, const_cast<char*>(g_pszKey_DESCRIPCION),
                              &description);
    CSF_AT(int, scenario, 0x3C) =
        MenuExtras_ParseDescriptionId(Campaign_GetTemporaryStringText(description));

    AssignStringRange_429B60(
        &description, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    Campaign_CopyPropertyText(properties, const_cast<char*>(g_pszKey_BRIEFING_CAMPAGNA),
                              &description);
    CSF_AT(int, scenario, 0x5C) =
        MenuExtras_ParseDescriptionId(Campaign_GetTemporaryStringText(description));

    AssignStringRange_429B60(
        &description, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    Campaign_CopyPropertyText(properties, const_cast<char*>(g_pszKey_BRIEFING_CARGA),
                              &description);
    CSF_AT(int, scenario, 0x7C) =
        MenuExtras_ParseDescriptionId(Campaign_GetTemporaryStringText(description));

    Campaign_CopyPropertyText(
        properties, const_cast<char*>(g_pszKey_FICHERO_DESCRIPCION), scenario + 0x40);
    Campaign_CopyPropertyText(
        properties, const_cast<char*>(g_pszKey_FICHERO_BRIEFING_CAMPAGNA), scenario + 0x60);
    Campaign_CopyPropertyText(
        properties, const_cast<char*>(g_pszKey_FICHERO_BRIEFING_CARGA), scenario + 0x80);
    Campaign_CopyPropertyText(
        properties, const_cast<char*>(g_pszKey_PANTALLA_CARGA), scenario + 0x9C);
    Campaign_CopyPropertyText(
        properties, const_cast<char*>(g_pszKey_ESCENARIO_INICIO), scenario + 0x20);

    alignas(8) std::uint8_t lastMissionBytes[0x1C];
    C_0082d410& lastMission =
        *reinterpret_cast<C_0082d410*>(lastMissionBytes);
    Campaign_ResetTemporaryString(lastMission);
    Campaign_CopyPropertyText(properties, const_cast<char*>(g_pszKey_LAST_MISSION),
                              &lastMission);
    CSF_AT(std::uint8_t, scenario, 0xC0) =
        std::strcmp(Campaign_GetTemporaryStringText(lastMission),
                    CSF_STR(0x00830FE0, "1")) == 0;

    alignas(8) std::uint8_t scenarioIdBytes[0x1C];
    C_0082d410& scenarioId =
        *reinterpret_cast<C_0082d410*>(scenarioIdBytes);
    Campaign_ResetTemporaryString(scenarioId);
    Campaign_CopyPropertyText(properties, const_cast<char*>(g_pszKey_ESCENARIO_ID),
                              &scenarioId);

    int result;
    if (CompareStringSubstring(
            &scenarioId, nullptr, 0, CSF_AT(unsigned int, &scenarioId, 0x14),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        result = ParseThumbnailIndex(Campaign_GetTemporaryStringText(scenarioId));
    }
    else
    {
        result = -1;
    }
    CSF_AT(int, scenario, 0x00) = result;

    if (CSF_AT(unsigned int, &scenarioId, 0x18) >= 0x10)
    {
        result = static_cast<int>(scenarioId.field_0004);
        if (scenarioId.field_0004)
            result = Mem_FreeDebug(reinterpret_cast<void*>(scenarioId.field_0004));
    }
    Campaign_ResetTemporaryString(scenarioId);

    if (CSF_AT(unsigned int, &lastMission, 0x18) >= 0x10)
    {
        result = static_cast<int>(lastMission.field_0004);
        if (lastMission.field_0004)
            result = Mem_FreeDebug(reinterpret_cast<void*>(lastMission.field_0004));
    }
    Campaign_ResetTemporaryString(lastMission);

    if (CSF_AT(unsigned int, &description, 0x18) >= 0x10)
    {
        result = static_cast<int>(description.field_0004);
        if (description.field_0004)
            result = Mem_FreeDebug(reinterpret_cast<void*>(description.field_0004));
    }
    return result;
}

HOOK(0x0043FC60, Campaign_LoadScenarioProperties, "auto");
