#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(void*, __fastcall, MenuLoading_FindMission, 0x0043E4B0,
         (void* database, void* edx, const char* name));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* destination, const char* edx, const char* source, uint32_t length));
CSF_FUNC(void*, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(int, __fastcall, MenuScenarioSelect_LookupString, 0x0069BC50,
         (void* database, void* edx, const char* name));
CSF_FUNC(uint16_t*, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440,
         (uint32_t identifier, int language));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0,
         (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* storage));

CSF_FUNC(void*, __cdecl, MenuLoading_GetMissionName, 0x00538F40,
         (void* destination));

// Hex-Rays' a1@<edi> (here and on 0x538F40) is the callee-save `push edi`: the function takes only its two
// stack arguments.
static uint16_t* __cdecl MenuLoading_GetLoadingText(uint16_t* destination, int capacity)
{
    *destination = 0;

    if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0) != 0)
    {
        const uint32_t mode =
            CSF_AT(uint32_t, GetGlobalState_50D190(), 0xCC);

        if (mode == 0)
        {
            const uint16_t* text =
                MenuMessageBox_ResolveEntryByKey(0x67313339, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }
        else if (mode == 1)
        {
            const uint16_t* text =
                MenuMessageBox_ResolveEntryByKey(0x67313430, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }
        else if (mode == 3)
        {
            const uint16_t* text =
                MenuMessageBox_ResolveEntryByKey(0x67313432, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }

        return destination;
    }

    // Observed MSVC string layout: base word, inline storage, size, capacity.
    uint32_t missionName[7];
    missionName[6] = 15;
    missionName[5] = 0;
    *reinterpret_cast<uint8_t*>(&missionName[1]) = 0;

    MenuLoading_GetMissionName(missionName);

    const char* missionKey =
        reinterpret_cast<const char*>(missionName[1]);
    if (missionName[6] < 16)
        missionKey = reinterpret_cast<const char*>(&missionName[1]);

    void* database = GetGlobal8afc7c();
    void* mission = MenuLoading_FindMission(database, nullptr, missionKey);

    if (mission != nullptr)
    {
        const char* languageSource;
        if (CSF_AT(uint32_t, mission, 0x98) < 16)
            languageSource =
                reinterpret_cast<const char*>(mission) + 0x84;
        else
            languageSource = CSF_AT(const char*, mission, 0x84);

        uint32_t languageName[7];
        languageName[6] = 15;
        languageName[5] = 0;
        *reinterpret_cast<uint8_t*>(&languageName[1]) = 0;

        uint32_t length = 0;
        while (languageSource[length] != 0)
            ++length;

        AssignStringRange_429B60(
            languageName, languageSource, languageSource, length);

        const char* languageKey =
            reinterpret_cast<const char*>(languageName[1]);
        if (languageName[6] < 16)
            languageKey = reinterpret_cast<const char*>(&languageName[1]);

        void* languages = GetGlobalValue_69B700();
        int language = MenuScenarioSelect_LookupString(languages, nullptr, languageKey);
        if (language == 0)
            language = 8;

        const uint32_t identifier = CSF_AT(uint32_t, mission, 0x7C);
        const uint16_t* text =
            MenuMessageBox_ResolveEntryByKey(identifier, language);
        MenuLoading_CopyLocalizedText(destination, text, capacity);

        if (languageName[6] >= 16)
        {
            void* storage = reinterpret_cast<void*>(languageName[1]);
            if (storage != nullptr)
                Mem_FreeDebug(storage);
        }
    }

    if (missionName[6] >= 16)
    {
        void* storage = reinterpret_cast<void*>(missionName[1]);
        if (storage != nullptr)
            Mem_FreeDebug(storage);
    }

    return destination;
}

HOOK(0x00539340, MenuLoading_GetLoadingText, "auto");
