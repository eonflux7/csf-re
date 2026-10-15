#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void, __stdcall, MenuMpServerInfo_FormatPlayerCount, 0x00530B10, (int category, int enabled, std::uint16_t* destination));
CSF_FUNC(void*, __stdcall, FormatCountText, 0x00530CD0, (int infoObject, void* outString));
CSF_FUNC(int, __cdecl, ComputeSpawnPointBalance, 0x005516C0, (uint16_t total, uint16_t first_deduction, uint16_t second_deduction));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __fastcall, C_00714270_ComputeDoubledFieldOffset, 0x00714270, (void* self, void*));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static std::uint16_t* MenuMpServerInfo_GetPlayerName(void* player)
{
    if (CSF_AT(std::uint32_t, player, 0x1C) < 8u)
        return reinterpret_cast<std::uint16_t*>(
            static_cast<std::uint8_t*>(player) + 8);
    return CSF_AT(std::uint16_t*, player, 8);
}

static void* MenuMpServerInfo_GetStringText(void* stringObject)
{
    if (CSF_AT(std::uint32_t, stringObject, 0x18) < 8u)
        return static_cast<std::uint8_t*>(stringObject) + 4;
    return CSF_AT(void*, stringObject, 4);
}

static int MenuMpServerInfo_SetRowText(
    CGuiListBoxBase* list, std::uint32_t row, int column, void* text)
{
    return reinterpret_cast<
        int(__fastcall*)(void*, void*, std::uint32_t, int, void*, int)
    >(list->vtbl[0x294 / 4])(list, nullptr, row, column, text, 0);
}

static void* MenuMpServerInfo_FormatNumericCell(
    void* destination, std::uint32_t first, std::uint32_t second,
    int numberMode, bool ratio)
{
    // Bytes not initialized by SetPattern must not leak unrelated stack data.
    alignas(8) std::uint8_t patternBytes[0x1C] = {};

    auto* patternText = ratio
        ? CSF_STR(0x00841B30, "{d0}/{d1}")
        : CSF_STR(0x00841B3C, "{d0} ");

    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(patternBytes), nullptr,
        reinterpret_cast<std::uint8_t*>(const_cast<char*>(patternText)));

    // The recovered final char parameter is actually the beginning of
    // a full-width argument list, read by the formatter through its address.
    using VariadicFormatter = void* (__cdecl*)(
        void*, char, void*, int, int, int, int, std::uint32_t, ...);
    auto formatter = reinterpret_cast<VariadicFormatter>(FormatCountTextBuffer);

    if (ratio) {
        return formatter(
            destination,
            CSF_AT(char, patternBytes, 0),
            CSF_AT(void*, patternBytes, 4),
            CSF_AT(int, patternBytes, 8),
            CSF_AT(int, patternBytes, 0xC),
            CSF_AT(int, patternBytes, 0x10),
            CSF_AT(int, patternBytes, 0x14),
            CSF_AT(std::uint32_t, patternBytes, 0x18),
            first, second, numberMode);
    }

    return formatter(
        destination,
        CSF_AT(char, patternBytes, 0),
        CSF_AT(void*, patternBytes, 4),
        CSF_AT(int, patternBytes, 8),
        CSF_AT(int, patternBytes, 0xC),
        CSF_AT(int, patternBytes, 0x10),
        CSF_AT(int, patternBytes, 0x14),
        CSF_AT(std::uint32_t, patternBytes, 0x18),
        first, numberMode);
}

static std::uint16_t* __fastcall CMenuMpServerInfo_AddPlayerRow(
    CMenuMpServerInfo* self, void* /*edx*/, int infoObject)
{
    void* player = reinterpret_cast<void*>(infoObject);
    std::uint16_t* result = MenuMpServerInfo_GetPlayerName(player);
    if (!*result)
        return result;

    CGuiListBoxBase* list;
    std::uint32_t* rowCount;
    const int category = CSF_AT(int, player, 0);

    if (category == 1 || category == 2) {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0010);
        rowCount = &CSF_AT(std::uint32_t, self, 0x1F8);
    } else {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_001c);
        rowCount = &CSF_AT(std::uint32_t, self, 0x204);
    }

    MenuMpServerInfo_SetRowText(
        list, *rowCount, 1, MenuMpServerInfo_GetPlayerName(player));

    alignas(8) std::uint8_t countTextBytes[0x200];
    MenuMpServerInfo_FormatPlayerCount(
        CSF_AT(int, player, 0x34),
        CSF_AT(int, player, 0),
        reinterpret_cast<std::uint16_t*>(countTextBytes));
    MenuMpServerInfo_SetRowText(list, *rowCount, 0, countTextBytes);

    alignas(8) std::uint8_t sizeBytes[0xC];
    auto* oldSize = reinterpret_cast<CGuiControl*>(list->vGetSize());
    CSF_AT(void*, sizeBytes, 0) = oldSize->vtbl;
    CSF_AT(std::uint32_t, sizeBytes, 4) = oldSize->field_0004;
    const int rowHeight = C_00714270_ComputeDoubledFieldOffset(list, nullptr);
    CSF_AT(std::uint32_t, sizeBytes, 4) += rowHeight;
    list->vSetSize(reinterpret_cast<int*>(sizeBytes));

    alignas(8) std::uint8_t stringBytes[0x1C];

    if (*MenuMpServerInfo_GetPlayerName(player) &&
        CSF_AT(int, player, 0) != 0) {
        CSF_AT(int, stringBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, stringBytes, 4) = 0;
        CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

        const int balance = ComputeSpawnPointBalance(
            CSF_AT(std::uint16_t, player, 0x20),
            CSF_AT(std::uint16_t, player, 0x22),
            CSF_AT(std::uint16_t, player, 0x28));
        void* formatted = MenuMpServerInfo_FormatNumericCell(
            stringBytes, static_cast<std::uint32_t>(balance), 0,
            g_NumberFormattingMode, false);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetRowText(
            list, *rowCount, 2, MenuMpServerInfo_GetStringText(stringBytes));

        formatted = MenuMpServerInfo_FormatNumericCell(
            stringBytes, CSF_AT(std::uint16_t, player, 0x20), 0,
            g_NumberFormattingMode, false);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetRowText(
            list, *rowCount, 3, MenuMpServerInfo_GetStringText(stringBytes));

        const std::uint32_t total =
            CSF_AT(std::uint16_t, player, 0x24) +
            CSF_AT(std::uint16_t, player, 0x26) +
            CSF_AT(std::uint16_t, player, 0x28);
        formatted = MenuMpServerInfo_FormatNumericCell(
            stringBytes, total, 0, g_NumberFormattingMode, false);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetRowText(
            list, *rowCount, 4, MenuMpServerInfo_GetStringText(stringBytes));

        formatted = FormatCountText(infoObject, stringBytes);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetRowText(
            list, *rowCount, 5, MenuMpServerInfo_GetStringText(stringBytes));

        result = reinterpret_cast<std::uint16_t*>(
            SaveGame_DestroyLocalizedString(stringBytes, nullptr));
        ++*rowCount;
    } else {
        CSF_AT(int, stringBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, stringBytes, 4) = 0;
        CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

        void* formatted = MenuMpServerInfo_FormatNumericCell(
            stringBytes,
            CSF_AT(std::uint32_t, player, 0x2C),
            CSF_AT(std::uint32_t, player, 0x30),
            g_NumberFormattingMode, true);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        AssignStringRange_488830(
            stringBytes, nullptr, stringBytes, 0, 0xFFFFFFFF);

        result = reinterpret_cast<std::uint16_t*>(
            MenuMpServerInfo_SetRowText(
                list, *rowCount, 5,
                MenuMpServerInfo_GetStringText(stringBytes)));

        if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8u) {
            result = CSF_AT(std::uint16_t*, stringBytes, 4);
            if (result)
                result = reinterpret_cast<std::uint16_t*>(Mem_FreeDebug(result));
        }
        ++*rowCount;
    }

    return result;
}

HOOK(0x00530D20, CMenuMpServerInfo_AddPlayerRow, "auto");
