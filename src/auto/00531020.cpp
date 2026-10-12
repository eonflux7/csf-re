#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void, __stdcall, MenuMpServerInfo_FormatStatusText, 0x00530B10, (int a1, int a2, uint16_t* a3));
CSF_FUNC(void*, __stdcall, FormatCountText, 0x00530CD0, (int infoObject, void* outString));
CSF_FUNC(int, __cdecl, ComputeSpawnPointBalance, 0x005516C0, (uint16_t total, uint16_t first_deduction, uint16_t second_deduction));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __fastcall, C_00714270_ComputeDoubledFieldOffset, 0x00714270, (void* self, void*));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static std::uint16_t* MenuMpServerInfo_GetPlayerName(void* info)
{
    if (CSF_AT(std::uint32_t, info, 0x1C) < 8u)
        return reinterpret_cast<std::uint16_t*>(
            reinterpret_cast<std::uint8_t*>(info) + 8);

    return CSF_AT(std::uint16_t*, info, 0x08);
}

static int MenuMpServerInfo_SetListCellText(
    CGuiListBoxBase* list, std::uint32_t row, int column, void* text)
{
    return reinterpret_cast<
        int(__fastcall*)(void*, void*, std::uint32_t, int, void*, int)
    >(list->vtbl[0x294 / 4])(list, nullptr, row, column, text, 0);
}

static void* MenuMpServerInfo_GetStringText(void* string)
{
    if (CSF_AT(std::uint32_t, string, 0x18) < 8u)
        return reinterpret_cast<std::uint8_t*>(string) + 4;

    return CSF_AT(void*, string, 0x04);
}

template<typename... Values>
static void* MenuMpServerInfo_FormatCounts(
    void* outString, const char* pattern, Values... values)
{
    alignas(8) std::uint8_t patternBytes[0x1C]{};
    FormatterSetPattern(patternBytes, nullptr, pattern);

    // The legacy formatter takes a 28-byte string by value, followed by
    // variadic formatting values. Its published binding exposes only the
    // output argument, so marshal the remaining stack arguments explicitly.
    using Formatter = void* (__cdecl*)(
        void*, char, void*, int, int, int, int, std::uint32_t, ...);

    return reinterpret_cast<Formatter>(FormatCountTextBuffer)(
        outString,
        CSF_AT(char, patternBytes, 0x00),
        CSF_AT(void*, patternBytes, 0x04),
        CSF_AT(int, patternBytes, 0x08),
        CSF_AT(int, patternBytes, 0x0C),
        CSF_AT(int, patternBytes, 0x10),
        CSF_AT(int, patternBytes, 0x14),
        CSF_AT(std::uint32_t, patternBytes, 0x18),
        values...);
}

static std::uint16_t* __fastcall CMenuMpServerInfo_AppendPlayerInfo(
    CMenuMpServerInfo* self, void* /*edx*/, int infoObject)
{
    void* info = reinterpret_cast<void*>(infoObject);
    std::uint16_t* result = MenuMpServerInfo_GetPlayerName(info);
    if (*result == 0)
        return result;

    CGuiListBoxBase* list;
    std::uint32_t* rowCount;

    if (CSF_AT(int, info, 0x00) == 1)
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0014);
        rowCount = &CSF_AT(std::uint32_t, self, 0x1FC);
    }
    else if (CSF_AT(int, info, 0x00) == 2)
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0018);
        rowCount = &CSF_AT(std::uint32_t, self, 0x200);
    }
    else
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_001c);
        rowCount = &CSF_AT(std::uint32_t, self, 0x204);
    }

    MenuMpServerInfo_SetListCellText(
        list, *rowCount, 1, MenuMpServerInfo_GetPlayerName(info));

    alignas(8) std::uint8_t statusTextBytes[0x200];
    MenuMpServerInfo_FormatStatusText(
        CSF_AT(int, info, 0x34),
        CSF_AT(int, info, 0x00),
        reinterpret_cast<std::uint16_t*>(statusTextBytes));

    MenuMpServerInfo_SetListCellText(list, *rowCount, 0, statusTextBytes);

    alignas(8) std::uint8_t sizeBytes[0x0C];
    auto* currentSize = reinterpret_cast<CGuiControl*>(list->vGetSize());
    CSF_AT(void**, sizeBytes, 0x00) = currentSize->vtbl;
    CSF_AT(std::uint32_t, sizeBytes, 0x04) = currentSize->field_0004;
    CSF_AT(std::uint32_t, sizeBytes, 0x04) +=
        C_00714270_ComputeDoubledFieldOffset(list, nullptr);
    list->vSetSize(reinterpret_cast<int*>(sizeBytes));

    // The four-byte prefix and adjacent 24-byte payload are one string.
    alignas(8) std::uint8_t stringBytes[0x1C]{};

    if (*MenuMpServerInfo_GetPlayerName(info) != 0 &&
        CSF_AT(int, info, 0x00) != 0)
    {
        CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, stringBytes, 0x04) = 0;
        CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

        const auto secondDeduction = CSF_AT(std::uint16_t, info, 0x28);
        const auto firstDeduction = CSF_AT(std::uint16_t, info, 0x22);
        const auto total = CSF_AT(std::uint16_t, info, 0x20);
        const int balance =
            ComputeSpawnPointBalance(total, firstDeduction, secondDeduction);
        const int balanceMode = g_NumberFormattingMode;

        void* formatted = MenuMpServerInfo_FormatCounts(
            stringBytes, CSF_STR(0x00841B3C, "{d0} "), balance, balanceMode);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetListCellText(
            list, *rowCount, 2, MenuMpServerInfo_GetStringText(stringBytes));

        const int totalMode = g_NumberFormattingMode;
        const std::uint32_t currentTotal =
            CSF_AT(std::uint16_t, info, 0x20);
        formatted = MenuMpServerInfo_FormatCounts(
            stringBytes, CSF_STR(0x00841B3C, "{d0} "), currentTotal, totalMode);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetListCellText(
            list, *rowCount, 3, MenuMpServerInfo_GetStringText(stringBytes));

        const std::uint32_t firstCount = CSF_AT(std::uint16_t, info, 0x24);
        const std::uint32_t secondCount = CSF_AT(std::uint16_t, info, 0x26);
        const std::uint32_t thirdCount = CSF_AT(std::uint16_t, info, 0x28);
        const int combinedMode = g_NumberFormattingMode;
        formatted = MenuMpServerInfo_FormatCounts(
            stringBytes,
            CSF_STR(0x00841B3C, "{d0} "),
            firstCount + secondCount + thirdCount,
            combinedMode);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetListCellText(
            list, *rowCount, 4, MenuMpServerInfo_GetStringText(stringBytes));

        formatted = FormatCountText(infoObject, stringBytes);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetListCellText(
            list, *rowCount, 5, MenuMpServerInfo_GetStringText(stringBytes));

        result = reinterpret_cast<std::uint16_t*>(
            SaveGame_DestroyLocalizedString(stringBytes, nullptr));
        ++*rowCount;
    }
    else
    {
        const std::uint32_t secondCount =
            CSF_AT(std::uint32_t, info, 0x30);
        const int formattingMode = g_NumberFormattingMode;
        CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, stringBytes, 0x04) = 0;
        const std::uint32_t firstCount =
            CSF_AT(std::uint32_t, info, 0x2C);
        CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

        void* formatted = MenuMpServerInfo_FormatCounts(
            stringBytes,
            CSF_STR(0x00841B30, "{d0}/{d1}"),
            firstCount,
            secondCount,
            formattingMode);
        AssignStringRange_488830(
            stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        AssignStringRange_488830(
            stringBytes, nullptr, stringBytes, 0, 0xFFFFFFFF);

        result = reinterpret_cast<std::uint16_t*>(
            MenuMpServerInfo_SetListCellText(
                list, *rowCount, 5,
                MenuMpServerInfo_GetStringText(stringBytes)));

        if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8u)
        {
            void* block = CSF_AT(void*, stringBytes, 0x04);
            result = reinterpret_cast<std::uint16_t*>(block);
            if (block)
                result = reinterpret_cast<std::uint16_t*>(Mem_FreeDebug(block));
        }
        ++*rowCount;
    }

    return result;
}

HOOK(0x00531020, CMenuMpServerInfo_AppendPlayerInfo, "auto");
