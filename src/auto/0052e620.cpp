#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(uint32_t, __fastcall, Campaign_ClearStrings, 0x0043E360, (uint32_t* self, void*));
CSF_FUNC(void*, __fastcall, CampaignTable_FindEntryById, 0x0043E450, (void* self, void* , int wantedId));
CSF_FUNC(int, __fastcall, Campaign_LookupBriefingEntry, 0x0043E590, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, ClearCampaignEntryList, 0x0043F890, (void* self, void*));
CSF_FUNC(int, __fastcall, Campaign_ResetSlotFields, 0x00440C20, (void* self, void*));
CSF_FUNC(std::uint32_t*, __fastcall, S_00440ee0_AppendElement196, 0x004413B0, (S_00440ee0* self, void* , int element));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __fastcall, MenuScenarioSelect_AddCampaignEntry, 0x0052E520, (void* self, void* edx, int a3, int arg4, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static void MenuScenarioSelect_InitializeString(C_0082d410& string)
{
    CSF_AT(std::uint32_t, &string, 0x18) = 15;
    CSF_AT(std::uint32_t, &string, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&string.field_0004)[0] = 0;
}

static char* MenuScenarioSelect_GetStringData(C_0082d410& string)
{
    if (CSF_AT(std::uint32_t, &string, 0x18) < 0x10)
        return reinterpret_cast<char*>(&string.field_0004);
    return reinterpret_cast<char*>(string.field_0004);
}

static std::uint32_t MenuScenarioSelect_ReleaseString(
    C_0082d410& string, std::uint32_t result)
{
    if (CSF_AT(std::uint32_t, &string, 0x18) >= 0x10)
    {
        result = string.field_0004;
        if (string.field_0004)
            result = Mem_FreeDebug(reinterpret_cast<void*>(string.field_0004));
    }
    return result;
}

static std::uint32_t __fastcall MenuScenarioSelect_RebuildCampaignList(
    void* self, void* /*edx*/)
{
    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x54);
    if (!result)
        return result;

    auto* list = reinterpret_cast<CGuiListBoxBase*>(result);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        list->vtbl[0x290 / 4])(list, nullptr);

    auto* entryList = reinterpret_cast<std::uint8_t*>(self) + 0x44;
    ClearCampaignEntryList(entryList, nullptr);

    alignas(8) std::uint8_t entryBytes[0xC4];
    alignas(8) std::uint8_t briefingNameBytes[0x1C];
    alignas(8) std::uint8_t briefingSceneBytes[0x1C];
    alignas(8) std::uint8_t currentSceneBytes[0x1C];
    alignas(8) std::uint8_t defaultSceneBytes[0x30];

    auto& briefingName = *reinterpret_cast<C_0082d410*>(briefingNameBytes);
    auto& briefingScene = *reinterpret_cast<C_0082d410*>(briefingSceneBytes);
    auto& currentScene = *reinterpret_cast<C_0082d410*>(currentSceneBytes);
    auto& defaultScene = *reinterpret_cast<C_0082d410*>(defaultSceneBytes);

    int nextIndex = 0;
    int currentEntryId = -1000;

    int settings = GetGlobalValue_8BC1E0();
    MenuScenarioSelect_InitializeString(currentScene);
    AssignStringSubstring(
        &currentScene, nullptr,
        reinterpret_cast<std::uint8_t*>(settings) + 1164, 0, -1);

    if (CompareStringSubstring(
            &currentScene, nullptr, 0,
            CSF_AT(std::uint32_t, &currentScene, 0x14),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        Campaign_ResetSlotFields(entryBytes, nullptr);
        CSF_AT(int, entryBytes, 0) = -1000;
        AssignStringSubstring(entryBytes + 4, nullptr, &currentScene, 0, -1);
        CSF_AT(int, entryBytes, 0x3C) =
            MenuExtras_ParseDescriptionId(CSF_STR(0x008416EC, "g105"));
        CSF_AT(int, entryBytes, 0xB8) = 0;
        MenuScenarioSelect_AddCampaignEntry(
            self, nullptr, reinterpret_cast<int>(entryBytes), 0,
            reinterpret_cast<int>(&currentScene));
        currentEntryId = CSF_AT(int, CSF_AT(void*, self, 0x48), 0);
        nextIndex = 1;
        Campaign_ClearStrings(reinterpret_cast<uint32_t*>(entryBytes), nullptr);
    }

    void* campaignTable = reinterpret_cast<void*>(GetGlobal8afc7c());
    auto* briefing = reinterpret_cast<std::uint8_t*>(
        Campaign_LookupBriefingEntry(campaignTable, nullptr, 0));

    MenuScenarioSelect_InitializeString(briefingName);
    AssignStringSubstring(&briefingName, nullptr, briefing, 0, -1);
    MenuScenarioSelect_InitializeString(briefingScene);
    AssignStringSubstring(&briefingScene, nullptr, briefing + 28, 0, -1);
    MenuScenarioSelect_InitializeString(defaultScene);
    AssignStringSubstring(&defaultScene, nullptr, &briefingScene, 0, -1);

    int defaultEntryId = -2000;
    if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 210))
    {
        char* currentName = MenuScenarioSelect_GetStringData(currentScene);
        char* defaultName = MenuScenarioSelect_GetStringData(defaultScene);
        if (SceneNamesMatch(defaultName, currentName))
        {
            defaultEntryId = currentEntryId;
        }
        else
        {
            Campaign_ResetSlotFields(entryBytes, nullptr);
            CSF_AT(int, entryBytes, 0) = -2000;
            AssignStringSubstring(entryBytes + 4, nullptr, &defaultScene, 0, -1);
            CSF_AT(int, entryBytes, 0x3C) =
                MenuExtras_ParseDescriptionId(CSF_STR(0x008416EC, "g105"));
            CSF_AT(int, entryBytes, 0xB8) = 0;
            MenuScenarioSelect_AddCampaignEntry(
                self, nullptr, reinterpret_cast<int>(entryBytes), nextIndex,
                reinterpret_cast<int>(&defaultScene));
            defaultEntryId = CSF_AT(
                int, CSF_AT(void*, self, 0x48), 196 * nextIndex);
            ++nextIndex;
            Campaign_ClearStrings(reinterpret_cast<uint32_t*>(entryBytes), nullptr);
        }
    }

    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 210))
    {
        auto* table = reinterpret_cast<std::uint8_t*>(GetGlobal8afc7c()) + 48;
        std::uint32_t begin = CSF_AT(std::uint32_t, table, 4);
        int count = begin
            ? static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, table, 8) - begin) / 196
            : 0;
        if (count)
        {
            std::uint32_t offset = 0;
            std::uint32_t remaining = static_cast<std::uint32_t>(count);
            do
            {
                auto* entry = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, table, 4)) + offset;
                if (currentEntryId != CSF_AT(int, entry, 0))
                {
                    int description = CSF_AT(int, entry, 0xB8);
                    list = CSF_AT(CGuiListBoxBase*, self, 0x54);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
                        list->vtbl[0x28C / 4])(
                            list, nullptr, nextIndex, 0, description, nextIndex);
                    S_00440ee0_AppendElement196(
                        reinterpret_cast<S_00440ee0*>(entryList), nullptr,
                        reinterpret_cast<int>(entry));
                    ++nextIndex;
                }
                offset += 196;
                --remaining;
            } while (remaining);
        }
    }
    else
    {
        void* profileManager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        int* scenarios = ProfileSavegames_GetScenarioCollection(
            CSF_AT(void*, profileManager, 8), nullptr);
        std::uint32_t begin = CSF_AT(std::uint32_t, scenarios, 4);
        if (begin)
        {
            // Original signed reciprocal multiplication for the 80-byte stride.
            std::int32_t byteCount = static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, scenarios, 8) - begin);
            std::int32_t count = static_cast<std::int32_t>(
                (std::int64_t{0x66666667} * byteCount) >> 32) >> 5;
            count += static_cast<std::uint32_t>(count) >> 31;
            if (count)
            {
                profileManager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
                void* profile = CSF_AT(void*, profileManager, 8);
                void* profileData =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, profile, 0)[36 / 4])(profile, nullptr);
                auto* unlockedScenarios =
                    reinterpret_cast<std::uint8_t*>(profileData) + 52;
                begin = CSF_AT(std::uint32_t, unlockedScenarios, 4);
                count = begin
                    ? static_cast<std::int32_t>(
                          CSF_AT(std::uint32_t, unlockedScenarios, 8) - begin) >> 4
                    : 0;
                if (count)
                {
                    std::uint32_t offset = 0;
                    std::uint32_t remaining = static_cast<std::uint32_t>(count);
                    do
                    {
                        int scenarioId = CSF_AT(
                            int, CSF_AT(void*, unlockedScenarios, 4), offset);
                        if (currentEntryId != scenarioId &&
                            defaultEntryId != scenarioId)
                        {
                            Campaign_ResetSlotFields(entryBytes, nullptr);
                            campaignTable = reinterpret_cast<void*>(GetGlobal8afc7c());
                            void* entry = CampaignTable_FindEntryById(
                                campaignTable, nullptr, scenarioId);
                            MenuScenarioSelect_AddCampaignEntry(
                                self, nullptr, reinterpret_cast<int>(entryBytes),
                                nextIndex++,
                                reinterpret_cast<int>(
                                    reinterpret_cast<std::uint8_t*>(entry) + 4));
                            Campaign_ClearStrings(
                                reinterpret_cast<uint32_t*>(entryBytes), nullptr);
                        }
                        offset += 16;
                        --remaining;
                    } while (remaining);
                }
            }
        }
    }

    CGuiListBoxBase_AddSelectedItem(
        CSF_AT(CGuiListBoxBase*, self, 0x54), nullptr, 0);

    result = CSF_AT(std::uint32_t, &defaultScene, 0x18);
    result = MenuScenarioSelect_ReleaseString(defaultScene, result);
    result = MenuScenarioSelect_ReleaseString(briefingScene, result);
    result = MenuScenarioSelect_ReleaseString(briefingName, result);
    return MenuScenarioSelect_ReleaseString(currentScene, result);
}

HOOK(0x0052E620, MenuScenarioSelect_RebuildCampaignList, "auto");
