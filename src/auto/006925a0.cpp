#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter consumes a 28-byte string by value, followed by three
// formatting arguments. Its supplied binding exposes only the first argument.
using FormatCountStackCall = void* (__cdecl*)(
    void*,
    std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t,
    std::uint32_t, std::uint32_t, std::uint32_t,
    int, int, int);

static void* __cdecl CoreParser_FormatDescriptionCount(
    void* outString,
    const char* description,
    int count,
    int formattingArgument,
    int entryFlag)
{
    const int descriptionId = MenuExtras_ParseDescriptionId(description);
    auto* entry = reinterpret_cast<std::uint16_t*>(
        MenuMessageBox_ResolveEntryByKey(descriptionId, entryFlag));

    alignas(8) std::uint8_t textBytes[0x1C]{};
    CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
    CSF_AT(int, textBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, textBytes, 4) = 0;

    if (!entry)
        return outString;

    if (MenuMessageBox_TestEntryAgainstFlag(
            entry, reinterpret_cast<char*>(&g_byte_82E65C)))
    {
        MenuProgressInput_AssignText(
            reinterpret_cast<std::uint32_t*>(textBytes),
            nullptr,
            reinterpret_cast<std::int16_t*>(entry));

        const int formattingMode = g_NumberFormattingMode;

        alignas(8) std::uint8_t argumentBytes[0x1C]{};
        CSF_AT(std::uint32_t, argumentBytes, 0x18) = 7;
        CSF_AT(int, argumentBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, argumentBytes, 4) = 0;

        AssignStringRange_488830(
            argumentBytes, nullptr, textBytes, 0, 0xFFFFFFFF);

        reinterpret_cast<FormatCountStackCall>(FormatCountTextBuffer)(
            outString,
            CSF_AT(std::uint32_t, argumentBytes, 0),
            CSF_AT(std::uint32_t, argumentBytes, 4),
            CSF_AT(std::uint32_t, argumentBytes, 8),
            CSF_AT(std::uint32_t, argumentBytes, 0xC),
            CSF_AT(std::uint32_t, argumentBytes, 0x10),
            CSF_AT(std::uint32_t, argumentBytes, 0x14),
            CSF_AT(std::uint32_t, argumentBytes, 0x18),
            count,
            formattingArgument,
            formattingMode);
    }

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, textBytes, 4);
        if (block)
            Mem_FreeDebug(block);
    }

    return outString;
}

HOOK(0x006925A0, CoreParser_FormatDescriptionCount, "auto");
