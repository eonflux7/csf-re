#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, IsMpStatsDisplayActive, 0x0042DFA0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_SetVisibleByState, 0x00540890, (CMenuMpScoreBar* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_RefreshChildVisibility, 0x00540940, (CMenuMpScoreBar* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, MpSpawn_QuerySelectedSpawnState, 0x005514B0, ());
CSF_FUNC(int, __fastcall, MpSpawn_GetRemainingSpawnDelay_551780, 0x00551780, (int* self, void*));
CSF_FUNC(int, __fastcall, MpSpawn_GetRemainingSpawnDelay_5517F0, 0x005517F0, (int* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_SpawnCountdownOffset, 0x00844624);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static inline __attribute__((always_inline))
wchar_t* MenuMpStats_GetWideStringData(void* string)
{
    if (CSF_AT(std::uint32_t, string, 0x18) < 8)
        return reinterpret_cast<wchar_t*>(static_cast<uint8_t*>(string) + 4);
    return CSF_AT(wchar_t*, string, 4);
}

static inline __attribute__((always_inline))
void MenuMpStats_FormatRespawnText(
    void* output, void* copy, wchar_t* pattern,
    int argumentCount, int first, int second, int formattingMode)
{
    ConstructWideString(copy, nullptr, pattern);

    // Pass the 28-byte string by value, then full-width formatting arguments.
    using FormatCall = void* (__cdecl*)(
        void*, char, void*, int, int, int, int, std::uint32_t, int, ...);

    if (argumentCount == 2)
    {
        reinterpret_cast<FormatCall>(FormatCountTextBuffer)(
            output, CSF_AT(char, copy, 0), CSF_AT(void*, copy, 4),
            CSF_AT(int, copy, 8), CSF_AT(int, copy, 0xC),
            CSF_AT(int, copy, 0x10), CSF_AT(int, copy, 0x14),
            CSF_AT(std::uint32_t, copy, 0x18),
            first, second, formattingMode);
    }
    else if (argumentCount == 1)
    {
        reinterpret_cast<FormatCall>(FormatCountTextBuffer)(
            output, CSF_AT(char, copy, 0), CSF_AT(void*, copy, 4),
            CSF_AT(int, copy, 8), CSF_AT(int, copy, 0xC),
            CSF_AT(int, copy, 0x10), CSF_AT(int, copy, 0x14),
            CSF_AT(std::uint32_t, copy, 0x18),
            first, formattingMode);
    }
    else
    {
        reinterpret_cast<FormatCall>(FormatCountTextBuffer)(
            output, CSF_AT(char, copy, 0), CSF_AT(void*, copy, 4),
            CSF_AT(int, copy, 8), CSF_AT(int, copy, 0xC),
            CSF_AT(int, copy, 0x10), CSF_AT(int, copy, 0x14),
            CSF_AT(std::uint32_t, copy, 0x18), formattingMode);
    }
    // The formatter releases the by-value copy's allocation.
}

static inline __attribute__((always_inline))
void MenuMpStats_BuildTeamRespawnText(
    bool allied, void* output, void* source, void* copy)
{
    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(output), nullptr, &g_byte_82E65C);

    int spawnState = GetGlobal8B0688();
    int count = allied
        ? CSF_AT(int, reinterpret_cast<void*>(spawnState), 0x143C)
        : CSF_AT(int, reinterpret_cast<void*>(spawnState), 0x1438);
    int rules = GetGlobalValue_558150();

    if (count < CSF_AT(int, reinterpret_cast<void*>(rules), 0x1AC))
    {
        wchar_t* entry = reinterpret_cast<wchar_t*>(
            CoreParser_ResolveEntryByKey(
                allied ? 1296904241 : 1296904240, 2));
        ConstructWideString(source, nullptr, entry);

        rules = GetGlobalValue_558150();
        spawnState = GetGlobal8B0688();
        count = allied
            ? CSF_AT(int, reinterpret_cast<void*>(spawnState), 0x143C)
            : CSF_AT(int, reinterpret_cast<void*>(spawnState), 0x1438);
        wchar_t* pattern = MenuMpStats_GetWideStringData(source);
        int formattingMode = g_NumberFormattingMode;
        int limit = CSF_AT(int, reinterpret_cast<void*>(rules), 0x1AC);

        MenuMpStats_FormatRespawnText(
            output, copy, pattern, 2, count, limit, formattingMode);
    }
    else
    {
        int* spawn = reinterpret_cast<int*>(GetGlobal8B0688());
        int delay = allied
            ? MpSpawn_GetRemainingSpawnDelay_551780(spawn, nullptr)
            : MpSpawn_GetRemainingSpawnDelay_5517F0(spawn, nullptr);

        if (delay == -1)
        {
            wchar_t* entry = reinterpret_cast<wchar_t*>(
                CoreParser_ResolveEntryByKey(
                    allied ? 1296904243 : 1296904242, 2));
            ConstructWideString(source, nullptr, entry);
            wchar_t* pattern = MenuMpStats_GetWideStringData(source);
            int formattingMode = g_NumberFormattingMode;
            MenuMpStats_FormatRespawnText(
                output, copy, pattern, 0, 0, 0, formattingMode);
        }
        else
        {
            wchar_t* entry = reinterpret_cast<wchar_t*>(
                CoreParser_ResolveEntryByKey(
                    allied ? 1296904245 : 1296904244, 2));
            ConstructWideString(source, nullptr, entry);

            int countdownOffset = g_SpawnCountdownOffset;
            spawn = reinterpret_cast<int*>(GetGlobal8B0688());
            delay = allied
                ? MpSpawn_GetRemainingSpawnDelay_551780(spawn, nullptr)
                : MpSpawn_GetRemainingSpawnDelay_5517F0(spawn, nullptr);
            int seconds = static_cast<int32_t>(
                static_cast<uint32_t>(delay) -
                static_cast<uint32_t>(countdownOffset)) / 1000;
            if (seconds < 0)
                seconds = 0;

            wchar_t* pattern = MenuMpStats_GetWideStringData(source);
            int formattingMode = g_NumberFormattingMode;
            MenuMpStats_FormatRespawnText(
                output, copy, pattern, 1, seconds, 0, formattingMode);
        }
    }
    SaveGame_DestroyLocalizedString(source, nullptr);
}

static int __fastcall CMenuMpScoreBar_UpdateStatusText(
    CMenuMpScoreBar* self, void* /*edx*/, int /*a2*/)
{
    alignas(8) uint8_t sourceBytes[0x1C] = {};
    alignas(8) uint8_t outputBytes[0x1C] = {};
    alignas(8) uint8_t copyBytes[0x1C] = {};
    char timeBuffer[0xC];

    CMenuMpScoreBar_SetVisibleByState(self, nullptr);
    GetGlobal8afc78();
    if (static_cast<uint8_t>(IsMpStatsDisplayActive()) != self->field_0038)
        CMenuMpScoreBar_RefreshChildVisibility(self, nullptr);

    int result = GetGlobal8B0688();
    if (!result)
        return result;

    if (self->field_0014)
    {
        int minutes = 0;
        int seconds = 0;
        GetGlobal8B0688();
        int remaining = MpSpawn_QuerySelectedSpawnState();
        if (remaining)
        {
            minutes = remaining / 60000;
            seconds = remaining % 60000 / 1000;
        }
        _sprintf(timeBuffer, CSF_STR(0x00841B54, "%02d:%02d"),
                 minutes, seconds);

        void* widget = reinterpret_cast<void*>(self->field_0014);
        void* text = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, widget, 0)[0x110 / 4])(widget, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, text, 0)[0x3C / 4])(text, nullptr, timeBuffer);
    }

    result = GetGlobalState_50D190();
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0xCC) != 3)
        return result;

    if (self->field_0018)
    {
        MenuMpStats_BuildTeamRespawnText(
            false, outputBytes, sourceBytes, copyBytes);

        void* widget = reinterpret_cast<void*>(self->field_0018);
        void* text = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, widget, 0)[0x110 / 4])(widget, nullptr);
        wchar_t* value = MenuMpStats_GetWideStringData(outputBytes);
        reinterpret_cast<void (__fastcall*)(void*, void*, wchar_t*)>(
            CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, value);
        SaveGame_DestroyLocalizedString(outputBytes, nullptr);
    }

    result = static_cast<int>(self->field_001c);
    if (!result)
        return result;

    MenuMpStats_BuildTeamRespawnText(
        true, outputBytes, sourceBytes, copyBytes);

    void* widget = reinterpret_cast<void*>(self->field_001c);
    void* text = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0)[0x110 / 4])(widget, nullptr);
    wchar_t* value = MenuMpStats_GetWideStringData(outputBytes);
    reinterpret_cast<void (__fastcall*)(void*, void*, wchar_t*)>(
        CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, value);
    return SaveGame_DestroyLocalizedString(outputBytes, nullptr);
}

HOOK(0x00540BC0, CMenuMpScoreBar_UpdateStatusText, "auto");
