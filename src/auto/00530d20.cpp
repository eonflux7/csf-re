#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void, __stdcall, MenuMpServerInfo_FormatPlayerLabel, 0x00530B10, (int a1, int a2, uint16_t* a3));
CSF_FUNC(void*, __stdcall, FormatCountText, 0x00530CD0, (int infoObject, void* outString));
CSF_FUNC(int, __cdecl, ComputeSpawnPointBalance, 0x005516C0, (uint16_t total, uint16_t first_deduction, uint16_t second_deduction));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __fastcall, C_00714270_ComputeDoubledFieldOffset, 0x00714270, (void* self, void*));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter receives a seven-dword string by value, followed by
// its formatting arguments. Its nominal binding omits those stack arguments.
struct CountTextPatternArgument {
    std::uint32_t words[7];
};

static void* MenuMpServerInfo_FormatNumber(void* output, int number)
{
    const int mode = g_NumberFormattingMode;
    alignas(8) std::uint8_t patternBytes[0x1C]{};
    FormatterSetPattern(patternBytes, nullptr, CSF_STR(0x00841B3C, "{d0} "));

    CountTextPatternArgument pattern;
    std::memcpy(&pattern, patternBytes, sizeof(pattern));
    return reinterpret_cast<void* (__cdecl*)(
        void*, CountTextPatternArgument, ...)>(FormatCountTextBuffer)(
            output, pattern, number, mode);
}

static void* MenuMpServerInfo_FormatRatio(void* output, int first, int second)
{
    const int mode = g_NumberFormattingMode;
    alignas(8) std::uint8_t patternBytes[0x1C]{};
    FormatterSetPattern(patternBytes, nullptr, CSF_STR(0x00841B30, "{d0}/{d1}"));

    CountTextPatternArgument pattern;
    std::memcpy(&pattern, patternBytes, sizeof(pattern));
    return reinterpret_cast<void* (__cdecl*)(
        void*, CountTextPatternArgument, ...)>(FormatCountTextBuffer)(
            output, pattern, first, second, mode);
}

static std::uint16_t* MenuMpServerInfo_GetInfoName(void* info)
{
    return CSF_AT(std::uint32_t, info, 0x1C) < 8
        ? reinterpret_cast<std::uint16_t*>(static_cast<std::uint8_t*>(info) + 8)
        : CSF_AT(std::uint16_t*, info, 8);
}

static void* MenuMpServerInfo_GetStringText(void* string)
{
    return CSF_AT(std::uint32_t, string, 0x18) < 8
        ? static_cast<std::uint8_t*>(string) + 4
        : CSF_AT(void*, string, 4);
}

static std::uint16_t* MenuMpServerInfo_SetCell(
    CGuiListBoxBase* list, std::uint32_t row, int column, void* text)
{
    return reinterpret_cast<std::uint16_t* (__fastcall*)(
        void*, void*, std::uint32_t, int, void*, int)>(
            list->vtbl[0x294 / 4])(list, nullptr, row, column, text, 0);
}

static std::uint16_t* __fastcall CMenuMpServerInfo_AddPlayerRow(
    CMenuMpServerInfo* self, void* /*edx*/, int infoObject)
{
    void* info = reinterpret_cast<void*>(infoObject);
    std::uint16_t* result = MenuMpServerInfo_GetInfoName(info);
    if (!*result)
        return result;

    CGuiListBoxBase* list;
    std::uint32_t* rowCount;
    const int team = CSF_AT(int, info, 0);
    if (team == 1 || team == 2) {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0010);
        rowCount = &self->field_01f8;
    } else {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_001c);
        rowCount = &self->field_0204;
    }

    MenuMpServerInfo_SetCell(
        list, *rowCount, 1, MenuMpServerInfo_GetInfoName(info));

    alignas(8) std::uint8_t labelBytes[0x200];
    MenuMpServerInfo_FormatPlayerLabel(
        CSF_AT(int, info, 0x34), CSF_AT(int, info, 0),
        reinterpret_cast<std::uint16_t*>(labelBytes));
    MenuMpServerInfo_SetCell(list, *rowCount, 0, labelBytes);

    alignas(8) std::uint8_t sizeBytes[0xC];
    auto* size = reinterpret_cast<CGuiControl*>(list->vGetSize());
    CSF_AT(std::uint32_t, sizeBytes, 0) =
        reinterpret_cast<std::uint32_t>(size->vtbl);
    CSF_AT(std::uint32_t, sizeBytes, 4) = size->field_0004;
    CSF_AT(std::uint32_t, sizeBytes, 4) +=
        C_00714270_ComputeDoubledFieldOffset(list, nullptr);
    list->vSetSize(reinterpret_cast<int*>(sizeBytes));

    const bool hasStatistics =
        *MenuMpServerInfo_GetInfoName(info) && CSF_AT(int, info, 0);

    // The four-byte prefix and the following 0x18 bytes form one string.
    alignas(8) std::uint8_t stringBytes[0x1C]{};
    CSF_AT(std::uint16_t, stringBytes, 4) = 0;
    CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

    if (hasStatistics) {
        const std::uint16_t secondDeduction =
            CSF_AT(std::uint16_t, info, 0x28);
        const std::uint16_t firstDeduction =
            CSF_AT(std::uint16_t, info, 0x22);
        const std::uint16_t total = CSF_AT(std::uint16_t, info, 0x20);
        const int balance =
            ComputeSpawnPointBalance(total, firstDeduction, secondDeduction);

        void* formatted = MenuMpServerInfo_FormatNumber(stringBytes, balance);
        AssignStringRange_488830(stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetCell(
            list, *rowCount, 2, MenuMpServerInfo_GetStringText(stringBytes));

        formatted = MenuMpServerInfo_FormatNumber(
            stringBytes, CSF_AT(std::uint16_t, info, 0x20));
        AssignStringRange_488830(stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetCell(
            list, *rowCount, 3, MenuMpServerInfo_GetStringText(stringBytes));

        const int firstCount = CSF_AT(std::uint16_t, info, 0x24);
        const int secondCount = CSF_AT(std::uint16_t, info, 0x26);
        const int thirdCount = CSF_AT(std::uint16_t, info, 0x28);
        formatted = MenuMpServerInfo_FormatNumber(
            stringBytes, firstCount + secondCount + thirdCount);
        AssignStringRange_488830(stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetCell(
            list, *rowCount, 4, MenuMpServerInfo_GetStringText(stringBytes));

        formatted = FormatCountText(infoObject, stringBytes);
        AssignStringRange_488830(stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_SetCell(
            list, *rowCount, 5, MenuMpServerInfo_GetStringText(stringBytes));

        result = reinterpret_cast<std::uint16_t*>(
            SaveGame_DestroyLocalizedString(stringBytes, nullptr));
    } else {
        void* formatted = MenuMpServerInfo_FormatRatio(
            stringBytes, CSF_AT(int, info, 0x2C), CSF_AT(int, info, 0x30));
        AssignStringRange_488830(stringBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        AssignStringRange_488830(
            stringBytes, nullptr, stringBytes, 0, 0xFFFFFFFF);

        result = MenuMpServerInfo_SetCell(
            list, *rowCount, 5, MenuMpServerInfo_GetStringText(stringBytes));
        if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8) {
            result = CSF_AT(std::uint16_t*, stringBytes, 4);
            if (result)
                result = reinterpret_cast<std::uint16_t*>(Mem_FreeDebug(result));
        }
    }

    ++*rowCount;
    return result;
}

HOOK(0x00530D20, CMenuMpServerInfo_AddPlayerRow, "auto");
