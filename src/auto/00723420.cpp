#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct WideStringArgument {
    std::uint32_t words[7];
};

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

CSF_FUNC(int, __fastcall, ConstructWideFormat, 0x00697100,
         (void* self, void* edx, const char* text));
CSF_FUNC(void*, __fastcall, CopyWideSubstring, 0x00488830,
         (void* self, void* edx, const void* source,
          std::uint32_t start, std::uint32_t length));
CSF_FUNC(void*, __cdecl, FormatWideNumber, 0x00696A80,
         (void* destination, WideStringArgument format, int value, int mode));
CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60, (void* block));

static void* __fastcall CGuiListBox_SetFormattedNumber(
    CGuiListBox* self, void* /*edx*/,
    int row, int column, int value, int options)
{
    WideStringArgument formatted;
    formatted.words[6] = 7;
    formatted.words[5] = 0;
    CSF_AT(std::uint16_t, &formatted, 4) = 0;

    WideStringArgument format;
    ConstructWideFormat(&format, nullptr, CSF_STR(0x008409A8, "{d0}"));

    const int mode = g_NumberFormattingMode;
    WideStringArgument formatArgument;
    formatArgument.words[6] = 7;
    formatArgument.words[5] = 0;
    CSF_AT(std::uint16_t, &formatArgument, 4) = 0;
    CopyWideSubstring(&formatArgument, nullptr, &format, 0, UINT32_MAX);
    FormatWideNumber(&formatted, formatArgument, value, mode);

    void* text = reinterpret_cast<void*>(formatted.words[1]);
    if (formatted.words[6] < 8)
        text = &formatted.words[1];

    void* result =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int, int, void*, int)>(
            CSF_AT(void**, self, 0)[0x294 / 4])(
                self, nullptr, row, column, text, options);

    if (format.words[6] >= 8) {
        result = reinterpret_cast<void*>(format.words[1]);
        if (result)
            result = reinterpret_cast<void*>(FreeDebugMemory(result));
    }

    format.words[6] = 7;
    format.words[5] = 0;
    CSF_AT(std::uint16_t, &format, 4) = 0;

    if (formatted.words[6] >= 8) {
        result = reinterpret_cast<void*>(formatted.words[1]);
        if (result)
            result = reinterpret_cast<void*>(FreeDebugMemory(result));
    }
    return result;
}

HOOK(0x00723420, CGuiListBox_SetFormattedNumber, "auto");
