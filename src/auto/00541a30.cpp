#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(bool, __cdecl, MpSpawn_IsCharacterSelectStateActive, 0x005515A0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static void* __cdecl MenuMpStats_FormatCount(
    void* output, void* pattern, int count, int mode, bool includeMode)
{
    // arg20 starts a variable argument list containing full-width integers.
    using Formatter = void* (__cdecl*)(
        void*, char, void*, int, int, int, int, std::uint32_t, ...);
    auto formatter = reinterpret_cast<Formatter>(FormatCountTextBuffer);

    if (includeMode) {
        return formatter(
            output, CSF_AT(char, pattern, 0),
            CSF_AT(void*, pattern, 4),
            CSF_AT(int, pattern, 8), CSF_AT(int, pattern, 0xC),
            CSF_AT(int, pattern, 0x10), CSF_AT(int, pattern, 0x14),
            CSF_AT(std::uint32_t, pattern, 0x18), count, mode);
    }
    return formatter(
        output, CSF_AT(char, pattern, 0),
        CSF_AT(void*, pattern, 4),
        CSF_AT(int, pattern, 8), CSF_AT(int, pattern, 0xC),
        CSF_AT(int, pattern, 0x10), CSF_AT(int, pattern, 0x14),
        CSF_AT(std::uint32_t, pattern, 0x18), count);
}

static int __fastcall CMenuMpStats_RefreshStatistics(
    CMenuMpStats* self, void* /*edx*/, int /*a2*/)
{
    int result = static_cast<int>(self->field_0010);
    if (!result)
        return result;
    result = GetGlobal8B0688();
    if (!result)
        return result;

    auto* statistics = reinterpret_cast<std::uint8_t*>(GetGlobal8B0688()) + 3240;
    void* list = reinterpret_cast<void*>(self->field_0010);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, list, 0)[0x298 / 4])(list, nullptr);

    alignas(8) std::uint8_t textBytes[0x28] = {};

    if (self->field_0014 && self->field_0018) {
        GetGlobal8B0688();
        if (MpSpawn_IsCharacterSelectStateActive()) {
            CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
            CSF_AT(int, textBytes, 0x14) = 0;
            CSF_AT(std::uint16_t, textBytes, 4) = 0;

            void* control = reinterpret_cast<void*>(self->field_0014);
            void* textControl = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
            int count = CSF_AT(std::uint16_t, statistics, 1792);

            alignas(8) std::uint8_t team1Pattern[0x1C] = {};
            ConstructWideString(
                team1Pattern, nullptr,
                reinterpret_cast<wchar_t*>(&self->field_001c));
            void* formatted = MenuMpStats_FormatCount(
                textBytes, team1Pattern, count, 0, false);
            void* text = CSF_AT(std::uint32_t, formatted, 0x18) < 8u
                ? static_cast<std::uint8_t*>(formatted) + 4
                : CSF_AT(void*, formatted, 4);
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, textControl, 0)[0x40 / 4])(
                    textControl, nullptr, text);

            control = reinterpret_cast<void*>(self->field_0018);
            textControl = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
            count = CSF_AT(std::uint16_t, statistics, 1794);

            alignas(8) std::uint8_t team2Pattern[0x1C] = {};
            ConstructWideString(
                team2Pattern, nullptr,
                reinterpret_cast<wchar_t*>(&self->field_004e));
            formatted = MenuMpStats_FormatCount(
                textBytes, team2Pattern, count, 0, false);
            text = CSF_AT(std::uint32_t, formatted, 0x18) < 8u
                ? static_cast<std::uint8_t*>(formatted) + 4
                : CSF_AT(void*, formatted, 4);
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, textControl, 0)[0x40 / 4])(
                    textControl, nullptr, text);
            SaveGame_DestroyLocalizedString(textBytes, nullptr);
        }
    }

    static constexpr unsigned int headerKeys[] = {
        1937008688, 1937008689, 1937008690, 1937008691,
        1937008692, 1937008693, 1937008694
    };
    for (int column = 0; column < 7; ++column) {
        list = reinterpret_cast<void*>(self->field_0010);
        void** table = CSF_AT(void**, list, 0);
        int heading = CoreParser_ResolveEntryByKey(headerKeys[column], 2);
        list = reinterpret_cast<void*>(self->field_0010);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
            table[0x294 / 4])(list, nullptr, 0, column, heading, 0);
    }

    auto* player = statistics + 8;
    unsigned int column = 0;
    int row = 1;
    int remaining = 32;
    do {
        std::uint32_t capacity = CSF_AT(std::uint32_t, player, 20);
        void* name = capacity < 8u ? player : CSF_AT(void*, player, 0);
        if (CSF_AT(std::uint16_t, name, 0)) {
            name = capacity < 8u ? player : CSF_AT(void*, player, 0);
            list = reinterpret_cast<void*>(self->field_0010);
            reinterpret_cast<void (__fastcall*)(void*, void*, int, unsigned int, void*, int)>(
                CSF_AT(void**, list, 0)[0x294 / 4])(
                    list, nullptr, row, 0, name, 0);

            name = CSF_AT(std::uint32_t, player, 20) < 8u
                ? player : CSF_AT(void*, player, 0);
            if (CSF_AT(std::uint16_t, name, 0)) {
                CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
                CSF_AT(int, textBytes, 0x14) = 0;
                CSF_AT(std::uint16_t, textBytes, 4) = 0;
                auto* value = player + 24;
                do {
                    int mode = g_NumberFormattingMode;
                    int count = CSF_AT(std::uint16_t, value, 0);
                    alignas(8) std::uint8_t patternBytes[0x1C] = {};
                    S_004f21d0_SetPattern(
                        reinterpret_cast<S_004f21d0*>(patternBytes), nullptr,
                        reinterpret_cast<std::uint8_t*>(
                            const_cast<char*>(CSF_STR(0x00841B3C, "{d0} "))));
                    void* formatted = MenuMpStats_FormatCount(
                        textBytes, patternBytes, count, mode, true);
                    AssignStringRange_488830(
                        textBytes, nullptr, formatted, 0, 0xFFFFFFFF);

                    void* text = CSF_AT(void*, textBytes, 4);
                    if (CSF_AT(std::uint32_t, textBytes, 0x18) < 8u)
                        text = textBytes + 4;
                    list = reinterpret_cast<void*>(self->field_0010);
                    reinterpret_cast<void (__fastcall*)(void*, void*, int, unsigned int, void*, int)>(
                        CSF_AT(void**, list, 0)[0x294 / 4])(
                            list, nullptr, row, ++column, text, 0);
                    value += 2;
                } while (column < 6);

                if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8u) {
                    void* block = CSF_AT(void*, textBytes, 4);
                    if (block)
                        Mem_FreeDebug(block);
                }
                CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
                CSF_AT(int, textBytes, 0x14) = 0;
                CSF_AT(std::uint16_t, textBytes, 4) = 0;
                column = 0;
            }
        }
        player += 56;
        result = remaining - 1;
        ++row;
        --remaining;
    } while (remaining != 0);
    return result;
}

HOOK(0x00541A30, CMenuMpStats_RefreshStatistics, "auto");
