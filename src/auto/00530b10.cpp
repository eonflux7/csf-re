#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void, __cdecl, AppendWideString, 0x00695700, (std::uint16_t* destination, const std::uint16_t* source));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));

// The formatter occupies seven dwords in the outgoing argument area.
struct FormatterArgument
{
    std::uint8_t bytes[0x1C];
};

static void __stdcall MenuMpServerInfo_FormatPlayerCount(
    int category, int enabled, std::uint16_t* destination)
{
    if (!destination)
        return;

    *destination = 0;
    if (!enabled)
        return;

    *destination = 0;

    int count;
    switch (category)
    {
    case 1:
    case 4:
        count = 22;
        break;
    case 2:
    case 5:
        count = 23;
        break;
    case 3:
    case 6:
        count = 21;
        break;
    default:
        return;
    }

    alignas(8) std::uint8_t stringBytes[0x1C];
    CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, stringBytes, 0x04) = 0;

    const int formattingMode = g_NumberFormattingMode;

    alignas(8) std::uint8_t formatterBytes[0x1C];
    CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;
    FormatterSetPattern(
        formatterBytes, nullptr, CSF_STR(0x00841B28, "@@{d0}/"));

    // Preserve the additional cdecl stack arguments omitted by the supplied
    // prototype: the by-value formatter, the count, and the formatting mode.
    using FormatWithArguments =
        void* (__cdecl*)(void*, FormatterArgument, int, int);
    reinterpret_cast<FormatWithArguments>(FormatCountTextBuffer)(
        stringBytes,
        *reinterpret_cast<FormatterArgument*>(formatterBytes),
        count,
        formattingMode);

    const std::uint16_t* text;
    if (CSF_AT(std::uint32_t, stringBytes, 0x18) < 8)
        text = reinterpret_cast<const std::uint16_t*>(stringBytes + 0x04);
    else
        text = reinterpret_cast<const std::uint16_t*>(
            CSF_AT(std::uint32_t, stringBytes, 0x04));

    AppendWideString(destination, text);
    SaveGame_DestroyLocalizedString(stringBytes, nullptr);
}

HOOK(0x00530B10, MenuMpServerInfo_FormatPlayerCount, "auto");
