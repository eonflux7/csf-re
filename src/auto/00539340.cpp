#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, MenuLoading_GetGlobalState, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, MenuLoading_GetMissionDatabase, 0x0043E010, ());
CSF_FUNC(void*, __fastcall, MenuLoading_FindMission, 0x0043E4B0,
         (void* database, void* edx, const char* name));
CSF_FUNC(void*, __fastcall, MenuLoading_AssignString, 0x00429B60,
         (void* destination, const char* edx, const char* source, uint32_t length));
CSF_FUNC(void*, __cdecl, MenuLoading_GetLanguageDatabase, 0x0069B700, ());
CSF_FUNC(int, __fastcall, MenuLoading_FindLanguage, 0x0069BC50,
         (void* database, void* edx, const char* name));
CSF_FUNC(uint16_t*, __cdecl, MenuLoading_GetLocalizedText, 0x00692440,
         (uint32_t identifier, int language));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0,
         (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(int, __cdecl, MenuLoading_FreeStringStorage, 0x00690E60,
         (void* storage));

// The additional input is EDI, not a stack argument.
CSF_FUNC(void*, __cdecl, MenuLoading_GetMissionName, 0x00538F40,
         (void* destination));

// Four x86 regcall arguments occupy EAX, ECX, EDX and EDI.
// Keep this bridge unoptimized: it forwards the EAX destination on the
// cdecl stack while leaving the incoming EDI selection in place.
static __attribute__((noinline, optnone))
void __regcall MenuLoading_ReadMissionName(
    void* destination, uint32_t /*ecxInput*/, uint32_t /*edxInput*/,
    int /*selection*/)
{
    MenuLoading_GetMissionName(destination);
}

// Only register arguments are declared, so this entry returns with RET 0.
// The original two stack arguments are read through the entry frame.
static __attribute__((no_caller_saved_registers))
uint16_t* __regcall MenuLoading_GetLoadingText(
    uint32_t /*eaxInput*/, uint32_t /*ecxInput*/, uint32_t /*edxInput*/,
    int selection, uint32_t /*esiInput*/)
{
    void* frame = __builtin_frame_address(0);
    uint16_t* destination = CSF_AT(uint16_t*, frame, 0x08);
    const int capacity = CSF_AT(int, frame, 0x0C);

    *destination = 0;

    if (CSF_AT(uint8_t, MenuLoading_GetGlobalState(), 0xD0) != 0)
    {
        const uint32_t mode =
            CSF_AT(uint32_t, MenuLoading_GetGlobalState(), 0xCC);

        if (mode == 0)
        {
            const uint16_t* text =
                MenuLoading_GetLocalizedText(0x67313339, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }
        else if (mode == 1)
        {
            const uint16_t* text =
                MenuLoading_GetLocalizedText(0x67313430, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }
        else if (mode == 3)
        {
            const uint16_t* text =
                MenuLoading_GetLocalizedText(0x67313432, 8);
            MenuLoading_CopyLocalizedText(destination, text, capacity);
        }

        return destination;
    }

    // Observed MSVC string layout: base word, inline storage, size, capacity.
    uint32_t missionName[7];
    missionName[6] = 15;
    missionName[5] = 0;
    *reinterpret_cast<uint8_t*>(&missionName[1]) = 0;

    MenuLoading_ReadMissionName(missionName, 0, 0, selection);

    const char* missionKey =
        reinterpret_cast<const char*>(missionName[1]);
    if (missionName[6] < 16)
        missionKey = reinterpret_cast<const char*>(&missionName[1]);

    void* database = MenuLoading_GetMissionDatabase();
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

        MenuLoading_AssignString(
            languageName, languageSource, languageSource, length);

        const char* languageKey =
            reinterpret_cast<const char*>(languageName[1]);
        if (languageName[6] < 16)
            languageKey = reinterpret_cast<const char*>(&languageName[1]);

        void* languages = MenuLoading_GetLanguageDatabase();
        int language = MenuLoading_FindLanguage(languages, nullptr, languageKey);
        if (language == 0)
            language = 8;

        const uint32_t identifier = CSF_AT(uint32_t, mission, 0x7C);
        const uint16_t* text =
            MenuLoading_GetLocalizedText(identifier, language);
        MenuLoading_CopyLocalizedText(destination, text, capacity);

        if (languageName[6] >= 16)
        {
            void* storage = reinterpret_cast<void*>(languageName[1]);
            if (storage != nullptr)
                MenuLoading_FreeStringStorage(storage);
        }
    }

    if (missionName[6] >= 16)
    {
        void* storage = reinterpret_cast<void*>(missionName[1]);
        if (storage != nullptr)
            MenuLoading_FreeStringStorage(storage);
    }

    return destination;
}

HOOK(0x00539340, MenuLoading_GetLoadingText, "auto");
