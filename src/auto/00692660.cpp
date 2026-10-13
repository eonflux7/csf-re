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

static int __cdecl CoreParser_FormatScenarioText(
    int outputString, int descriptionText, int count, int flag)
{
    const int code = MenuExtras_ParseDescriptionId(
        reinterpret_cast<const char*>(descriptionText));
    const int entry = MenuMessageBox_ResolveEntryByKey(code, flag);

    alignas(8) std::uint8_t textBytes[0x1C]{};
    CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
    CSF_AT(int, textBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, textBytes, 4) = 0;

    if (!entry)
        return outputString;

    if (MenuMessageBox_TestEntryAgainstFlag(
            reinterpret_cast<uint16_t*>(entry),
            reinterpret_cast<char*>(&g_byte_82E65C)))
    {
        MenuProgressInput_AssignText(
            reinterpret_cast<std::uint32_t*>(textBytes), nullptr,
            reinterpret_cast<std::int16_t*>(entry));

        const int formattingMode = g_NumberFormattingMode;

        alignas(8) std::uint8_t argumentBytes[0x1C]{};
        CSF_AT(std::uint32_t, argumentBytes, 0x18) = 7;
        CSF_AT(int, argumentBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, argumentBytes, 4) = 0;

        AssignStringRange_488830(
            argumentBytes, nullptr, textBytes, 0, 0xFFFFFFFF);

        // The binding omits the by-value string and formatting arguments.
        // Pass its seven stack dwords followed by count and formatting mode.
        using FormatWithArguments = void* (__cdecl*)(
            void*, std::uint32_t, std::uint32_t, std::uint32_t,
            std::uint32_t, std::uint32_t, std::uint32_t,
            std::uint32_t, int, int);

        reinterpret_cast<FormatWithArguments>(FormatCountTextBuffer)(
            reinterpret_cast<void*>(outputString),
            CSF_AT(std::uint32_t, argumentBytes, 0),
            CSF_AT(std::uint32_t, argumentBytes, 4),
            CSF_AT(std::uint32_t, argumentBytes, 8),
            CSF_AT(std::uint32_t, argumentBytes, 0xC),
            CSF_AT(std::uint32_t, argumentBytes, 0x10),
            CSF_AT(std::uint32_t, argumentBytes, 0x14),
            CSF_AT(std::uint32_t, argumentBytes, 0x18),
            count, formattingMode);
    }

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, textBytes, 4);
        if (block)
            Mem_FreeDebug(block);
    }

    return outputString;
}

HOOK(0x00692660, CoreParser_FormatScenarioText, "auto");
