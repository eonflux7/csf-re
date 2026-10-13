#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __fastcall, MpRules_LookupScenarioEntry, 0x00558340, (void* self, void* , int entryIndex, int bombMode, int teamColour));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

static unsigned int __fastcall MenuScenarioSelect_PopulateScenarioDescriptions(
    void* self, void* /*edx*/)
{
    void* rules = reinterpret_cast<std::uint8_t*>(GetGlobalValue_558150()) + 8;
    void* scenarioRegistry =
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);

    void* descriptionList = CSF_AT(void*, self, 0x30);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, descriptionList, 0)[0x2B8 / 4])(
            descriptionList, nullptr);

    unsigned int entryIndex = 0;
    for (; entryIndex < 0xA; ++entryIndex)
    {
        const int scenarioId = MpRules_LookupScenarioEntry(
            rules, nullptr, static_cast<int>(entryIndex),
            CSF_AT(int, self, 0x40), CSF_AT(int, self, 0x44));

        const int scenarioIndex =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, scenarioRegistry, 0)[0xC / 4])(
                    scenarioRegistry, nullptr, scenarioId);

        if (scenarioIndex == -1)
            continue;

        void* scenario =
            ((void*(__fastcall*)(void*, void*, int))
                CSF_AT(void**, scenarioRegistry, 0)[0x8 / 4])(
                    scenarioRegistry, nullptr, scenarioIndex);

        GetGlobal8B0688();
        switch (GetMpGameModeIndex())
        {
        case 0:
        case 1:
        case 3:
            break;

        case 2:
        {
            const int scenarioType = CSF_AT(int, scenario, 0x74);
            if (scenarioType <= 0 || scenarioType > 2)
                continue;
            break;
        }

        default:
            continue;
        }

        const char* descriptionText;
        if (CSF_AT(std::uint32_t, scenario, 0x38) < 0x10u)
            descriptionText =
                reinterpret_cast<const char*>(scenario) + 0x24;
        else
            descriptionText = CSF_AT(const char*, scenario, 0x24);

        void** listVtable =
            CSF_AT(void**, CSF_AT(void*, self, 0x30), 0);
        const int descriptionId =
            MenuExtras_ParseDescriptionId(descriptionText);

        ((void(__fastcall*)(void*, void*, int))
            listVtable[0x29C / 4])(
                CSF_AT(void*, self, 0x30), nullptr, descriptionId);
    }

    return entryIndex;
}

HOOK(0x0052F540, MenuScenarioSelect_PopulateScenarioDescriptions, "auto");
