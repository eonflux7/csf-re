#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CMenuMpServerInfo_SetChildStatesIfAllPresent, 0x00530660, (CMenuMpServerInfo* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpServerInfo_DisableChildControls, 0x00530710, (CMenuMpServerInfo* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpServerInfo_ShowPanelsForState, 0x00530770, (CMenuMpServerInfo* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpServerInfo_UpdateStatisticsLayout, 0x00530800, (CMenuMpServerInfo* self, void*));
CSF_FUNC(uint16_t*, __fastcall, CMenuMpServerInfo_AddPlayerRow, 0x00530D20, (void* self, void* edx, int a2));
CSF_FUNC(uint16_t*, __fastcall, CMenuMpServerInfo_AppendPlayerInfo, 0x00531020, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, CMenuMpServerInfo_InitializeStatisticHeaders, 0x00531440, (CMenuMpServerInfo* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(const char*, __fastcall, C_0082d410_FindSessionMapDefinition, 0x0054E630, (C_0082d410* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(char*, __cdecl, GetSelectedCharacterSpawnName, 0x00551310, ());
CSF_FUNC(int, __cdecl, MpSpawn_QuerySelectedSpawnState, 0x005514B0, ());
CSF_FUNC(bool, __cdecl, MpSpawn_IsCharacterSelectStateActive, 0x005515A0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(char*, __cdecl, MpSpawn_GetCharacterSelectNamePtr, 0x00551620, ());
CSF_FUNC(int, __fastcall, MpSpawn_QueryFlagForGlobal, 0x00551650, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, C_00714270_ComputeDoubledFieldOffset, 0x00714270, (void* self, void*));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static void* MenuMpServerInfo_GetTextControl(std::uint32_t controlAddress)
{
    void* control = reinterpret_cast<void*>(controlAddress);
    return ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
}

static void* MenuMpServerInfo_SetWideText(void* control, void* text)
{
    return ((void*(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, control, 0)[0x40 / 4])(control, nullptr, text);
}

static void MenuMpServerInfo_SetLocalizedText(void* control, unsigned int key)
{
    void** table = CSF_AT(void**, control, 0);
    int text = CoreParser_ResolveEntryByKey(key, 2);
    ((void(__fastcall*)(void*, void*, int))table[0x40 / 4])(
        control, nullptr, text);
}

static void MenuMpServerInfo_InitializeWideText(void* text)
{
    CSF_AT(std::uint32_t, text, 0x18) = 7;
    CSF_AT(int, text, 0x14) = 0;
    CSF_AT(std::uint16_t, text, 4) = 0;
}

static void* MenuMpServerInfo_GetWideTextData(void* text)
{
    if (CSF_AT(std::uint32_t, text, 0x18) < 8u)
        return static_cast<std::uint8_t*>(text) + 4;
    return CSF_AT(void*, text, 4);
}

template<typename... Args>
static void* MenuMpServerInfo_FormatWideText(
    void* output, wchar_t* format, Args... arguments)
{
    alignas(8) std::uint8_t formatBytes[0x1C]{};
    ConstructWideString(formatBytes, nullptr, format);

    // Preserve the full-width variadic arguments beyond the recovered
    // prototype's final byte argument.
    using Formatter = void*(__cdecl*)(
        void*, char, void*, int, int, int, int, std::uint32_t, ...);
    return reinterpret_cast<Formatter>(FormatCountTextBuffer)(
        output,
        CSF_AT(char, formatBytes, 0),
        CSF_AT(void*, formatBytes, 4),
        CSF_AT(int, formatBytes, 8),
        CSF_AT(int, formatBytes, 0xC),
        CSF_AT(int, formatBytes, 0x10),
        CSF_AT(int, formatBytes, 0x14),
        CSF_AT(std::uint32_t, formatBytes, 0x18),
        arguments...);
}

static void MenuMpServerInfo_ResizePlayerList(std::uint32_t& listAddress)
{
    alignas(8) std::uint8_t sizeBytes[0x8]{};
    auto* list = reinterpret_cast<CGuiListBoxBase*>(listAddress);
    auto* size = static_cast<CGuiControl*>(list->vGetSize());
    CSF_AT(std::uint32_t, sizeBytes, 0) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(size->vtbl));
    CSF_AT(int, sizeBytes, 4) = C_00714270_ComputeDoubledFieldOffset(
        reinterpret_cast<void*>(listAddress), nullptr);
    reinterpret_cast<CGuiListBoxBase*>(listAddress)->vSetSize(
        reinterpret_cast<int*>(sizeBytes));
}

static void* __fastcall CMenuMpServerInfo_ApplyReadyState(
    CMenuMpServerInfo* self, void* /*edx*/, char forceRefresh)
{
    bool refresh = false;
    if (GetGlobal8B0688())
    {
        void* session = reinterpret_cast<void*>(GetGlobal8B0688());
        const char dirty = CSF_AT(char, session, 0xC9C);
        CSF_AT(char, session, 0xC9C) = 0;
        refresh = dirty || forceRefresh;
    }

    if (refresh)
    {
        CMenuMpServerInfo_DisableChildControls(self, nullptr);
        CMenuMpServerInfo_ShowPanelsForState(self, nullptr);
        CMenuMpServerInfo_InitializeStatisticHeaders(self, nullptr);
        self->field_01f8 = 0;
        self->field_01fc = 0;
        self->field_0200 = 0;
        self->field_0204 = 0;

        auto* players = reinterpret_cast<std::uint8_t*>(GetGlobal8B0688()) + 3240;
        GetGlobal8B0688();
        if (MpSpawn_IsCharacterSelectStateActive())
        {
            GetGlobal8B0688();
            if (GetMpGameModeIndex() == 3)
            {
                alignas(8) std::uint8_t countTextBytes[0x1C]{};
                MenuMpServerInfo_InitializeWideText(countTextBytes);

                void* settings = reinterpret_cast<void*>(GetGlobalValue_558150());
                int redCount = CSF_AT(std::uint16_t, players, 0x704);
                if (redCount > CSF_AT(int, settings, 0x1AC))
                    redCount = CSF_AT(int,
                        reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1AC);

                settings = reinterpret_cast<void*>(GetGlobalValue_558150());
                int blueCount = CSF_AT(std::uint16_t, players, 0x706);
                if (blueCount > CSF_AT(int, settings, 0x1AC))
                    blueCount = CSF_AT(int,
                        reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1AC);

                void* label = MenuMpServerInfo_GetTextControl(self->field_0028);
                settings = reinterpret_cast<void*>(GetGlobalValue_558150());
                int mode = g_NumberFormattingMode;
                int maximum = CSF_AT(int, settings, 0x1AC);
                void* formatted = MenuMpServerInfo_FormatWideText(
                    countTextBytes, &CSF_AT(wchar_t, self, 0x118),
                    redCount, maximum, mode);
                MenuMpServerInfo_SetWideText(
                    label, MenuMpServerInfo_GetWideTextData(formatted));

                label = MenuMpServerInfo_GetTextControl(self->field_002c);
                settings = reinterpret_cast<void*>(GetGlobalValue_558150());
                mode = g_NumberFormattingMode;
                maximum = CSF_AT(int, settings, 0x1AC);
                formatted = MenuMpServerInfo_FormatWideText(
                    countTextBytes, &CSF_AT(wchar_t, self, 0x17C),
                    blueCount, maximum, mode);
                MenuMpServerInfo_SetWideText(
                    label, MenuMpServerInfo_GetWideTextData(formatted));
                SaveGame_DestroyLocalizedString(countTextBytes, nullptr);
            }
        }

        MenuMpServerInfo_ResizePlayerList(self->field_0010);
        MenuMpServerInfo_ResizePlayerList(self->field_0014);
        MenuMpServerInfo_ResizePlayerList(self->field_0018);
        MenuMpServerInfo_ResizePlayerList(self->field_001c);

        auto* player = players;
        for (int remaining = 32; remaining; --remaining, player += 56)
        {
            const std::uint32_t state = self->field_01e4;
            if (state == 0)
            {
                CMenuMpServerInfo_AddPlayerRow(
                    self, nullptr, reinterpret_cast<int>(player));
            }
            else if (state == 1 || state == 3)
            {
                CMenuMpServerInfo_AppendPlayerInfo(
                    self, nullptr, reinterpret_cast<int>(player));
            }
        }

        CMenuMpServerInfo_SetChildStatesIfAllPresent(self, nullptr);
        CMenuMpServerInfo_UpdateStatisticsLayout(self, nullptr);

        const std::uint32_t state = self->field_01e4;
        if (state == 0 || state == 1 || state == 3)
        {
            const unsigned int firstKey = state == 0 ? 1937125686
                : state == 1 ? 1937125687 : 1937125689;
            const unsigned int secondKey = state == 0 ? 1937125424
                : state == 1 ? 1937125425 : 1937125427;
            MenuMpServerInfo_SetLocalizedText(
                MenuMpServerInfo_GetTextControl(self->field_0044), firstKey);
            MenuMpServerInfo_SetLocalizedText(
                MenuMpServerInfo_GetTextControl(self->field_0048), secondKey);
        }

        if (self->field_01e0)
        {
            const std::uint32_t readyState = self->field_01e4;
            if (readyState == 0)
            {
                MenuMpServerInfo_SetLocalizedText(
                    MenuMpServerInfo_GetTextControl(self->field_0044), 1836068914);
            }
            else if (readyState == 1 || readyState == 3)
            {
                if (CSF_AT(int, players, 0x70C) == -1)
                {
                    MenuMpServerInfo_SetLocalizedText(
                        MenuMpServerInfo_GetTextControl(self->field_0044),
                        1937125941);
                }
                else
                {
                    void* session = reinterpret_cast<void*>(GetGlobal8B0688());
                    int flag = MpSpawn_QueryFlagForGlobal(session, nullptr);
                    const unsigned int key =
                        CSF_AT(int, players, 0x70C) == flag
                        ? 1937125938 : 1937125939;
                    MenuMpServerInfo_SetLocalizedText(
                        MenuMpServerInfo_GetTextControl(self->field_0044), key);
                }
            }
        }

        alignas(8) std::uint8_t mapTextBytes[0x1C]{};
        MenuMpServerInfo_InitializeWideText(mapTextBytes);

        GetGlobal8B0688();
        char* characterName = MpSpawn_GetCharacterSelectNamePtr();
        alignas(8) std::uint8_t mapNameBytes[0x1C]{};
        CSF_AT(std::uint32_t, mapNameBytes, 0x18) = 15;
        CSF_AT(int, mapNameBytes, 0x14) = 0;
        CSF_AT(char, mapNameBytes, 4) = 0;
        AssignStringRange_429B60(
            mapNameBytes, nullptr, characterName,
            static_cast<unsigned int>(std::strlen(characterName)));

        auto* mapDefinitions =
            reinterpret_cast<C_0082d410*>(GetGlobalDword_8B0680());
        const char* mapDefinition = C_0082d410_FindSessionMapDefinition(
            mapDefinitions, nullptr, reinterpret_cast<int>(mapNameBytes));
        if (CSF_AT(std::uint32_t, mapNameBytes, 0x18) >= 0x10 &&
            CSF_AT(void*, mapNameBytes, 4))
        {
            Mem_FreeDebug(CSF_AT(void*, mapNameBytes, 4));
        }

        if (mapDefinition)
        {
            void* label = MenuMpServerInfo_GetTextControl(self->field_003c);
            void* formatted = MenuMpServerInfo_FormatWideText(
                mapTextBytes, reinterpret_cast<wchar_t*>(&self->field_0050),
                mapDefinition);
            MenuMpServerInfo_SetWideText(
                label, MenuMpServerInfo_GetWideTextData(formatted));
        }

        GetGlobal8B0688();
        void* spawnLabel = MenuMpServerInfo_GetTextControl(self->field_0040);
        char* spawnName = GetSelectedCharacterSpawnName();
        ((void(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, spawnLabel, 0)[0x3C / 4])(
                spawnLabel, nullptr, spawnName + 135);

        reinterpret_cast<CGuiListBoxBase*>(self->field_001c)->vSetDisabled(
            self->field_0204 ? 0 : 1);

        if (CSF_AT(std::uint32_t, mapTextBytes, 0x18) >= 8 &&
            CSF_AT(void*, mapTextBytes, 4))
        {
            Mem_FreeDebug(CSF_AT(void*, mapTextBytes, 4));
        }
    }

    GetGlobal8B0688();
    const int time = MpSpawn_QuerySelectedSpawnState();
    int minutes = 0;
    int seconds = 0;
    if (time)
    {
        minutes = time / 60000;
        seconds = time % 60000 / 1000;
    }

    alignas(8) std::uint8_t timerBufferBytes[0x10]{};
    char* timerBuffer = reinterpret_cast<char*>(timerBufferBytes);
    _sprintf(timerBuffer, CSF_STR(0x00841B54, "%02d:%02d"), minutes, seconds);

    alignas(8) std::uint8_t timerTextBytes[0x1C]{};
    MenuMpServerInfo_InitializeWideText(timerTextBytes);
    void* timerLabel = MenuMpServerInfo_GetTextControl(self->field_0038);
    void* formatted = MenuMpServerInfo_FormatWideText(
        timerTextBytes, &CSF_AT(wchar_t, self, 0xB4), timerBuffer);
    void* result = MenuMpServerInfo_SetWideText(
        timerLabel, MenuMpServerInfo_GetWideTextData(formatted));

    if (CSF_AT(std::uint32_t, timerTextBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, timerTextBytes, 4);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x005322A0, CMenuMpServerInfo_ApplyReadyState, "auto");
