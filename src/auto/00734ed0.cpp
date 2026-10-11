#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __fastcall, ProfileName_CompareInlineOrHeap, 0x00515AB0, (void* self, void* , void* entry));
CSF_FUNC(int, __fastcall, MenuEula_FindWideSubstring, 0x00532BC0, (void* self, void* , std::uint16_t* pattern, std::uint64_t packedSearch));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));
CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

CSF_GLOBAL(char, g_OpeningBindingPattern, 0x008671C0);
CSF_GLOBAL(char, g_ClosingBindingPattern, 0x008671BC);
CSF_GLOBAL(char, g_MaxPlayersSettingValue, 0x008445D8);
CSF_GLOBAL(uint32_t, g_BindingCount, 0x008600D8);

// The packet also classifies this direct-call address as a vtable reference.
static const auto k_WideSubstringReference = CSF_VTBL(0x00532BC0);

static char __cdecl MenuEula_ParseBindingIndex(
    unsigned int* position, S_00532bc0* text, unsigned int* bindingIndex)
{
    const uint32_t initialPosition = *position;
    if (initialPosition == UINT32_MAX || initialPosition >= text->field_0014)
        return 0;

    // Keep the original scratch capacity, including the small conversion buffers.
    alignas(8) uint8_t scratch[0x284];
    char* fallbackText = reinterpret_cast<char*>(scratch);
    void* zeroPattern = scratch + 0x200;
    void* openingPattern = scratch + 0x21C;
    char* indexText = reinterpret_cast<char*>(scratch + 0x238);
    void* closingPattern = scratch + 0x240;
    void* extractedIndex = scratch + 0x25C;
    char* formattedCount = reinterpret_cast<char*>(scratch + 0x27C);

    FormatterSetPattern(openingPattern, nullptr, &g_OpeningBindingPattern);
    FormatterSetPattern(closingPattern, nullptr, &g_ClosingBindingPattern);

    auto* openingData = CSF_AT(std::uint16_t*, openingPattern, 0x04);
    if (CSF_AT(uint32_t, openingPattern, 0x18) < 8)
        openingData = reinterpret_cast<std::uint16_t*>(
            static_cast<uint8_t*>(openingPattern) + 0x04);

    uint32_t searchStart = *position;
    size_t patternLength = _wcslen(reinterpret_cast<const wchar_t*>(openingData));
    CSF_AT(uint32_t, formattedCount, 0) =
        static_cast<uint32_t>(MenuEula_FindWideSubstring(
            text, nullptr, openingData,
            (static_cast<uint64_t>(patternLength) << 32) | searchStart));

    auto* closingData = CSF_AT(std::uint16_t*, closingPattern, 0x04);
    if (CSF_AT(uint32_t, closingPattern, 0x18) < 8)
        closingData = reinterpret_cast<std::uint16_t*>(
            static_cast<uint8_t*>(closingPattern) + 0x04);

    searchStart = *position;
    patternLength = _wcslen(reinterpret_cast<const wchar_t*>(closingData));
    const uint32_t closingPosition =
        static_cast<uint32_t>(MenuEula_FindWideSubstring(
            text, nullptr, closingData,
            (static_cast<uint64_t>(patternLength) << 32) | searchStart));

    if (CSF_AT(uint32_t, formattedCount, 0) != UINT32_MAX)
    {
        if (closingPosition == UINT32_MAX)
        {
            auto* source = text->field_0018 < 8
                ? reinterpret_cast<std::uint16_t*>(&text->pad_0000[4])
                : CSF_AT(std::uint16_t*, text, 0x04);
            ConvertWideTextToNarrow(source, fallbackText, 512);
        }
        else if (closingPosition > CSF_AT(uint32_t, formattedCount, 0))
        {
            CSF_AT(uint32_t, extractedIndex, 0x14) = 0;
            CSF_AT(std::uint16_t, extractedIndex, 0x04) = 0;
            uint32_t characterPosition =
                CSF_AT(uint32_t, openingPattern, 0x14) +
                CSF_AT(uint32_t, formattedCount, 0);
            CSF_AT(uint32_t, extractedIndex, 0x18) = 7;

            while (characterPosition < closingPosition)
            {
                auto* source = text->field_0018 < 8
                    ? reinterpret_cast<std::uint16_t*>(&text->pad_0000[4])
                    : CSF_AT(std::uint16_t*, text, 0x04);
                const std::int16_t character =
                    static_cast<std::int16_t>(source[characterPosition++]);
                SaveSlots_ApplyIndex(extractedIndex, nullptr, character);
            }

            auto* extractedData =
                reinterpret_cast<std::uint16_t*>(
                    static_cast<uint8_t*>(extractedIndex) + 0x04);
            if (CSF_AT(uint32_t, extractedIndex, 0x18) >= 8)
                extractedData = CSF_AT(std::uint16_t*, extractedIndex, 0x04);

            ConvertWideTextToNarrow(
                extractedData, indexText,
                CSF_AT(uint32_t, extractedIndex, 0x14));
            const std::int32_t parsedIndex = ParseThumbnailIndex(indexText);

            bool rejectZero = false;
            if (parsedIndex == 0)
            {
                const int entry = FormatterSetPattern(
                    zeroPattern, nullptr, &g_MaxPlayersSettingValue);
                rejectZero = ProfileName_CompareInlineOrHeap(
                    extractedIndex, nullptr, reinterpret_cast<void*>(entry)) != 0;
                SaveGame_DestroyLocalizedString(zeroPattern, nullptr);
            }

            const uint32_t bindingCount = g_BindingCount;
            if (!rejectZero && parsedIndex >= 0 &&
                parsedIndex < static_cast<std::int32_t>(bindingCount))
            {
                *position =
                    CSF_AT(uint32_t, closingPattern, 0x14) +
                    CSF_AT(uint32_t, openingPattern, 0x14) +
                    CSF_AT(uint32_t, extractedIndex, 0x14) +
                    CSF_AT(uint32_t, formattedCount, 0);
                *bindingIndex = static_cast<uint32_t>(parsedIndex);
                SaveGame_DestroyLocalizedString(extractedIndex, nullptr);
                SaveGame_DestroyLocalizedString(closingPattern, nullptr);
                SaveGame_DestroyLocalizedString(openingPattern, nullptr);
                return 1;
            }

            _sprintf(formattedCount, CSF_STR(0x0083090C, "%d"), bindingCount);
            SaveGame_DestroyLocalizedString(extractedIndex, nullptr);
            SaveGame_DestroyLocalizedString(closingPattern, nullptr);
            SaveGame_DestroyLocalizedString(openingPattern, nullptr);
            return 0;
        }
    }

    if (CSF_AT(uint32_t, closingPattern, 0x18) >= 8 &&
        CSF_AT(void*, closingPattern, 0x04))
        Mem_FreeDebug(CSF_AT(void*, closingPattern, 0x04));

    const uint32_t openingCapacity =
        CSF_AT(uint32_t, openingPattern, 0x18);
    CSF_AT(uint32_t, closingPattern, 0x18) = 7;
    CSF_AT(uint32_t, closingPattern, 0x14) = 0;
    CSF_AT(std::uint16_t, closingPattern, 0x04) = 0;

    if (openingCapacity >= 8 && CSF_AT(void*, openingPattern, 0x04))
        Mem_FreeDebug(CSF_AT(void*, openingPattern, 0x04));
    return 0;
}

HOOK(0x00734ED0, MenuEula_ParseBindingIndex, "auto");
