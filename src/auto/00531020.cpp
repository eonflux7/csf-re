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

// The formatter's trailing arguments are full-width formatting values.
// Its recovered char arg20 marks the start of those variadic stack arguments.
using MenuMpServerInfo_VariadicFormatter =
    void* (__cdecl*)(void*, char, void*, int, int, int, int,
                     std::uint32_t, ...);

static void MenuMpServerInfo_FormatStatistic(void* text, std::uint32_t value)
{
    const int formattingMode = g_NumberFormattingMode;
    alignas(8) std::uint8_t patternBytes[0x1C] = {};

    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(patternBytes), nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x00841B3C, "{d0} "))));

    void* formatted = reinterpret_cast<MenuMpServerInfo_VariadicFormatter>(
        FormatCountTextBuffer)(
        text,
        CSF_AT(char, patternBytes, 0x00),
        CSF_AT(void*, patternBytes, 0x04),
        CSF_AT(int, patternBytes, 0x08),
        CSF_AT(int, patternBytes, 0x0C),
        CSF_AT(int, patternBytes, 0x10),
        CSF_AT(int, patternBytes, 0x14),
        CSF_AT(std::uint32_t, patternBytes, 0x18),
        value, formattingMode);

    AssignStringRange_488830(text, nullptr, formatted, 0, 0xFFFFFFFF);
}

static void* MenuMpServerInfo_GetTextData(void* text)
{
    if (CSF_AT(std::uint32_t, text, 0x18) < 8)
        return static_cast<std::uint8_t*>(text) + 0x04;
    return CSF_AT(void*, text, 0x04);
}

static int MenuMpServerInfo_WritePlayerColumn(
    CGuiListBoxBase* list, std::uint32_t row, int column, void* text)
{
    return reinterpret_cast<
        int (__fastcall*)(void*, void*, std::uint32_t, int, void*, int)>(
        list->vtbl[0x294 / 4])(list, nullptr, row, column, text, 0);
}

static std::uint16_t* __fastcall CMenuMpServerInfo_AppendPlayerInfo(
    CMenuMpServerInfo* self, void* /*edx*/, int infoObject)
{
    auto* info = reinterpret_cast<std::uint8_t*>(infoObject);
    auto* name = CSF_AT(std::uint32_t, info, 0x1C) < 8
        ? reinterpret_cast<std::uint16_t*>(info + 0x08)
        : CSF_AT(std::uint16_t*, info, 0x08);

    if (*name == 0)
        return name;

    CGuiListBoxBase* list;
    std::uint32_t* rowCount;
    const int team = CSF_AT(int, info, 0x00);
    if (team == 1)
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0014);
        rowCount = &CSF_AT(std::uint32_t, self, 0x1FC);
    }
    else if (team == 2)
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_0018);
        rowCount = &CSF_AT(std::uint32_t, self, 0x200);
    }
    else
    {
        list = reinterpret_cast<CGuiListBoxBase*>(self->field_001c);
        rowCount = &CSF_AT(std::uint32_t, self, 0x204);
    }

    name = CSF_AT(std::uint32_t, info, 0x1C) < 8
        ? reinterpret_cast<std::uint16_t*>(info + 0x08)
        : CSF_AT(std::uint16_t*, info, 0x08);
    MenuMpServerInfo_WritePlayerColumn(list, *rowCount, 1, name);

    alignas(8) std::uint8_t playerCountBytes[0x200];
    MenuMpServerInfo_FormatPlayerCount(
        CSF_AT(int, info, 0x34), CSF_AT(int, info, 0x00),
        reinterpret_cast<std::uint16_t*>(playerCountBytes));
    MenuMpServerInfo_WritePlayerColumn(list, *rowCount, 0, playerCountBytes);

    alignas(8) std::uint8_t sizeBytes[0xC];
    void* currentSize = list->vGetSize();
    CSF_AT(std::uint32_t, sizeBytes, 0x00) =
        CSF_AT(std::uint32_t, currentSize, 0x00);
    CSF_AT(std::uint32_t, sizeBytes, 0x04) =
        CSF_AT(std::uint32_t, currentSize, 0x04);
    CSF_AT(std::uint32_t, sizeBytes, 0x04) +=
        C_00714270_ComputeDoubledFieldOffset(list, nullptr);
    list->vSetSize(reinterpret_cast<int*>(sizeBytes));

    name = CSF_AT(std::uint32_t, info, 0x1C) < 8
        ? reinterpret_cast<std::uint16_t*>(info + 0x08)
        : CSF_AT(std::uint16_t*, info, 0x08);

    // The string spans all locals from ebp-0x1C through ebp-1.
    alignas(8) std::uint8_t textBytes[0x1C];
    CSF_AT(std::uint32_t, textBytes, 0x00) = 0;

    if (*name != 0 && CSF_AT(int, info, 0x00) != 0)
    {
        const std::uint16_t secondDeduction =
            CSF_AT(std::uint16_t, info, 0x28);
        CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, textBytes, 0x04) = 0;
        const std::uint16_t firstDeduction =
            CSF_AT(std::uint16_t, info, 0x22);
        const std::uint16_t total =
            CSF_AT(std::uint16_t, info, 0x20);
        CSF_AT(std::uint32_t, textBytes, 0x18) = 7;

        const int balance =
            ComputeSpawnPointBalance(total, firstDeduction, secondDeduction);
        MenuMpServerInfo_FormatStatistic(
            textBytes, static_cast<std::uint32_t>(balance));
        MenuMpServerInfo_WritePlayerColumn(
            list, *rowCount, 2, MenuMpServerInfo_GetTextData(textBytes));

        MenuMpServerInfo_FormatStatistic(
            textBytes, CSF_AT(std::uint16_t, info, 0x20));
        MenuMpServerInfo_WritePlayerColumn(
            list, *rowCount, 3, MenuMpServerInfo_GetTextData(textBytes));

        const std::uint32_t firstCount =
            CSF_AT(std::uint16_t, info, 0x24);
        const std::uint32_t secondCount =
            CSF_AT(std::uint16_t, info, 0x26);
        const std::uint32_t thirdCount =
            CSF_AT(std::uint16_t, info, 0x28);
        MenuMpServerInfo_FormatStatistic(
            textBytes, firstCount + secondCount + thirdCount);
        MenuMpServerInfo_WritePlayerColumn(
            list, *rowCount, 4, MenuMpServerInfo_GetTextData(textBytes));

        void* formatted = FormatCountText(infoObject, textBytes);
        AssignStringRange_488830(
            textBytes, nullptr, formatted, 0, 0xFFFFFFFF);
        MenuMpServerInfo_WritePlayerColumn(
            list, *rowCount, 5, MenuMpServerInfo_GetTextData(textBytes));

        auto* result = reinterpret_cast<std::uint16_t*>(
            SaveGame_DestroyLocalizedString(textBytes, nullptr));
        ++*rowCount;
        return result;
    }

    const std::uint32_t secondValue =
        CSF_AT(std::uint32_t, info, 0x30);
    const int formattingMode = g_NumberFormattingMode;
    CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, textBytes, 0x04) = 0;
    const std::uint32_t firstValue =
        CSF_AT(std::uint32_t, info, 0x2C);
    CSF_AT(std::uint32_t, textBytes, 0x18) = 7;

    alignas(8) std::uint8_t patternBytes[0x1C] = {};
    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(patternBytes), nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x00841B30, "{d0}/{d1}"))));

    void* formatted = reinterpret_cast<MenuMpServerInfo_VariadicFormatter>(
        FormatCountTextBuffer)(
        textBytes,
        CSF_AT(char, patternBytes, 0x00),
        CSF_AT(void*, patternBytes, 0x04),
        CSF_AT(int, patternBytes, 0x08),
        CSF_AT(int, patternBytes, 0x0C),
        CSF_AT(int, patternBytes, 0x10),
        CSF_AT(int, patternBytes, 0x14),
        CSF_AT(std::uint32_t, patternBytes, 0x18),
        firstValue, secondValue, formattingMode);

    AssignStringRange_488830(
        textBytes, nullptr, formatted, 0, 0xFFFFFFFF);
    AssignStringRange_488830(
        textBytes, nullptr, textBytes, 0, 0xFFFFFFFF);

    auto* result = reinterpret_cast<std::uint16_t*>(
        MenuMpServerInfo_WritePlayerColumn(
            list, *rowCount, 5, MenuMpServerInfo_GetTextData(textBytes)));

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, textBytes, 0x04);
        result = static_cast<std::uint16_t*>(block);
        if (block)
            result = reinterpret_cast<std::uint16_t*>(Mem_FreeDebug(block));
    }

    ++*rowCount;
    return result;
}

HOOK(0x00531020, CMenuMpServerInfo_AppendPlayerInfo, "auto");
