#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __cdecl, GetValue_8BAF98, 0x0069C370, ());

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter takes a 28-byte string by value, followed by format arguments.
// This trivial carrier preserves that ABI without invoking any string constructors.
struct PatternArgument
{
    std::uint8_t bytes[0x1C];
};

using FormatterCall = void* (__cdecl*)(void*, PatternArgument, ...);

static PatternArgument CopyPatternArgument(const void* pattern)
{
    PatternArgument argument;
    std::memcpy(argument.bytes, pattern, sizeof(argument.bytes));
    return argument;
}

static void InitializeEmptyString(void* string)
{
    CSF_AT(std::uint32_t, string, 0x18) = 7;
    CSF_AT(std::uint32_t, string, 0x14) = 0;
    CSF_AT(std::uint16_t, string, 0x04) = 0;
}

static const void* GetStringCharacters(void* string)
{
    if (CSF_AT(std::uint32_t, string, 0x18) < 8)
        return static_cast<std::uint8_t*>(string) + 0x04;
    return CSF_AT(void*, string, 0x04);
}

static void FreeStringStorage(void* string)
{
    if (CSF_AT(std::uint32_t, string, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, string, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }
}

static int __cdecl ProfilesSavegames_FormatDateTime(
    const void* dateTime, void* outString)
{
    alignas(8) std::uint8_t patternBytes[0x1C]{};
    alignas(8) std::uint8_t minuteBytes[0x1C]{};
    alignas(8) std::uint8_t secondBytes[0x1C]{};
    alignas(8) std::uint8_t yearBytes[0x1C]{};
    alignas(8) std::uint8_t argumentBytes[0x1C]{};

    int minute = CSF_AT(int, dateTime, 0x10);
    InitializeEmptyString(minuteBytes);
    const char* numberPattern = minute < 10
        ? CSF_STR(0x008409A0, "0{d0}")
        : CSF_STR(0x008409A8, "{d0}");
    int mode = g_NumberFormattingMode;
    FormatterSetPattern(argumentBytes, nullptr, numberPattern);
    reinterpret_cast<FormatterCall>(FormatCountTextBuffer)(
        minuteBytes, CopyPatternArgument(argumentBytes), minute, mode);

    int second = static_cast<std::int32_t>(
        static_cast<std::int64_t>(CSF_AT(float, dateTime, 0x14)));
    InitializeEmptyString(secondBytes);
    numberPattern = second < 10
        ? CSF_STR(0x008409A0, "0{d0}")
        : CSF_STR(0x008409A8, "{d0}");
    mode = g_NumberFormattingMode;
    FormatterSetPattern(argumentBytes, nullptr, numberPattern);
    reinterpret_cast<FormatterCall>(FormatCountTextBuffer)(
        secondBytes, CopyPatternArgument(argumentBytes), second, mode);

    int year = CSF_AT(int, dateTime, 0x00) % 2000;
    InitializeEmptyString(yearBytes);
    numberPattern = year < 10
        ? CSF_STR(0x008409A0, "0{d0}")
        : CSF_STR(0x008409A8, "{d0}");
    mode = g_NumberFormattingMode;
    FormatterSetPattern(argumentBytes, nullptr, numberPattern);
    reinterpret_cast<FormatterCall>(FormatCountTextBuffer)(
        yearBytes, CopyPatternArgument(argumentBytes), year, mode);

    FormatterSetPattern(
        patternBytes, nullptr,
        CSF_STR(0x00840980, "{d0}:{w1}:{w2} {d3}/{d4}/{w5}"));
    int dateFormat = GetValue_8BAF98();

    const void* yearText = GetStringCharacters(yearBytes);
    const void* secondText = GetStringCharacters(secondBytes);
    const void* minuteText = GetStringCharacters(minuteBytes);
    mode = g_NumberFormattingMode;

    int firstDateField;
    int secondDateField;
    if (dateFormat == 12)
    {
        secondDateField = CSF_AT(int, dateTime, 0x08);
        firstDateField = CSF_AT(int, dateTime, 0x04);
    }
    else
    {
        secondDateField = CSF_AT(int, dateTime, 0x04);
        firstDateField = CSF_AT(int, dateTime, 0x08);
    }
    int hour = CSF_AT(int, dateTime, 0x0C);

    InitializeEmptyString(argumentBytes);
    AssignStringRange_488830(
        argumentBytes, nullptr, patternBytes, 0, 0xFFFFFFFF);
    void* result = reinterpret_cast<FormatterCall>(FormatCountTextBuffer)(
        outString, CopyPatternArgument(argumentBytes),
        hour, minuteText, secondText,
        firstDateField, secondDateField, yearText, mode);

    FreeStringStorage(patternBytes);
    InitializeEmptyString(patternBytes);
    FreeStringStorage(yearBytes);
    InitializeEmptyString(yearBytes);
    FreeStringStorage(secondBytes);
    InitializeEmptyString(secondBytes);
    FreeStringStorage(minuteBytes);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x00514180, ProfilesSavegames_FormatDateTime, "auto");
