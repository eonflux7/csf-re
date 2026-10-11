#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(S_004f21d0*, __fastcall, S_004f21d0_ResetAndAssign, 0x00488BE0, (S_004f21d0* self, void* , void* a2));
CSF_FUNC(void*, __fastcall, Menu_ReplaceTextRange, 0x00533040, (void* self, void* edx, unsigned int a2, unsigned int a3, void* a4, unsigned int a5, unsigned int a6));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(char, __fastcall, Menu_GrowReplacementRanges, 0x00732DA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Menu_FindTokenEnd, 0x00733A80, (void* self, void* edx, uint16_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(char, __cdecl, Menu_FindNextIndexedToken, 0x00734ED0, (unsigned int* a1, void* a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter also receives a 28-byte string by value, followed by
// the count and formatting mode. These are absent from its packet binding.
struct alignas(4) MenuFormatStringArgument
{
    uint8_t bytes[0x1C];
};

static char __fastcall C_008670f0_ExpandIndexedText(
    C_008670f0* self, void* /*edx*/, S_00696af0* text)
{
    char result = static_cast<char>(self->field_00da);
    if (!result)
        return result;

    result = static_cast<char>(self->field_00d9);
    if (!result)
        return result;

    result = static_cast<char>(
        MenuAccount_GateCheck_696B60(text, nullptr, &g_byte_82E65C));
    if (result)
        return result;

    alignas(8) uint8_t formatWideBytes[0x200];
    alignas(8) uint8_t markerWideBytes[0x200];
    alignas(8) uint8_t formatNarrowBytes[0x100];
    alignas(8) uint8_t formatStringBytes[0x1C];
    alignas(8) uint8_t tokenStringBytes[0x24];
    alignas(8) uint8_t replacementStringBytes[0x24];

    auto* markerWide = reinterpret_cast<uint16_t*>(markerWideBytes);
    auto* formatNarrow = reinterpret_cast<char*>(formatNarrowBytes);
    auto& tokenString = *reinterpret_cast<S_004f21d0*>(tokenStringBytes);
    auto& replacementString =
        *reinterpret_cast<S_004f21d0*>(replacementStringBytes);

    self->field_00e4 = 0;
    CopyTrucoStringToWide(CSF_STR(0x008671C0, "@@"), markerWide, 256);
    _sprintf(formatNarrow, CSF_STR(0x008671C8, "%s{d0}%s"),
             CSF_STR(0x008671C0, "@@"), CSF_STR(0x008671BC, "/"));
    CopyTrucoStringToWide(
        formatNarrow, reinterpret_cast<uint16_t*>(formatWideBytes), 256);

    tokenString.field_0018 = 7;
    tokenString.field_0014 = 0;
    CSF_AT(uint16_t, tokenStringBytes, 0x04) = 0;
    replacementString.field_0018 = 7;
    replacementString.field_0014 = 0;
    CSF_AT(uint16_t, replacementStringBytes, 0x04) = 0;

    ConstructWideString(
        formatStringBytes, nullptr, reinterpret_cast<wchar_t*>(formatWideBytes));

    auto* definitions = reinterpret_cast<uint8_t*>(self->field_0008);
    const unsigned int definitionCount = CSF_AT(uint32_t, definitions, 0x18);
    auto* definitionStorage = definitions + 0x10;
    unsigned int position = 0;
    unsigned int tokenIndex = 0;

    using FormatCountWithArguments = void* (__cdecl*)(
        void*, MenuFormatStringArgument, unsigned int, int);

    if (Menu_FindNextIndexedToken(&position, text, &tokenIndex))
    {
        do
        {
            if (tokenIndex < definitionCount)
            {
                const unsigned int rowOffset = 516 * tokenIndex;
                auto* rows = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, definitionStorage, 0));

                if (CSF_AT(uint32_t, rows + rowOffset, 0) != tokenIndex)
                {
                    const size_t markerLength =
                        _wcslen(reinterpret_cast<wchar_t*>(markerWideBytes));
                    const unsigned int tokenEnd = static_cast<unsigned int>(
                        Menu_FindTokenEnd(text, nullptr, markerWide,
                                          position, markerLength));

                    TextInputSubobject_Init(
                        tokenStringBytes, nullptr, &g_byte_82E65C);

                    const int tokenMode = g_NumberFormattingMode;
                    const unsigned int formattedIndex = tokenIndex;
                    MenuFormatStringArgument tokenFormat{};
                    auto& tokenFormatString =
                        *reinterpret_cast<S_004f21d0*>(tokenFormat.bytes);
                    tokenFormatString.field_0018 = 7;
                    tokenFormatString.field_0014 = 0;
                    CSF_AT(uint16_t, tokenFormat.bytes, 0x04) = 0;
                    AssignStringRange_488830(
                        tokenFormat.bytes, nullptr, formatStringBytes,
                        0, 0xFFFFFFFF);
                    reinterpret_cast<FormatCountWithArguments>(
                        FormatCountTextBuffer)(
                            tokenStringBytes, tokenFormat,
                            formattedIndex, tokenMode);

                    rows = reinterpret_cast<uint8_t*>(
                        CSF_AT(uint32_t, definitionStorage, 0));
                    auto* row = rows + rowOffset;

                    if (CSF_AT(uint32_t, row, 0) == 0xFFFFFFFF)
                    {
                        MenuProgressInput_AssignText(
                            reinterpret_cast<uint32_t*>(replacementStringBytes),
                            nullptr, reinterpret_cast<int16_t*>(row + 4));

                        const unsigned int replacementLength =
                            replacementString.field_0014;
                        auto* ranges = reinterpret_cast<uint8_t*>(self) + 0xDC;
                        if (self->field_00e4 >= CSF_AT(uint32_t, self, 0xE0))
                        {
                            Menu_GrowReplacementRanges(
                                ranges, nullptr, CSF_AT(int, self, 0xE8));
                        }

                        const uint32_t rangeCount = self->field_00e4;
                        auto* range = reinterpret_cast<uint8_t*>(
                            CSF_AT(uint32_t, self, 0xDC)) + 8 * rangeCount;
                        self->field_00e4 = rangeCount + 1;
                        CSF_AT(uint32_t, range, 0) = tokenEnd;
                        CSF_AT(uint32_t, range, 4) = replacementLength;
                    }
                    else
                    {
                        TextInputSubobject_Init(
                            replacementStringBytes, nullptr, &g_byte_82E65C);

                        const int replacementMode = g_NumberFormattingMode;
                        rows = reinterpret_cast<uint8_t*>(
                            CSF_AT(uint32_t, definitionStorage, 0));
                        const unsigned int count =
                            CSF_AT(uint32_t, rows + rowOffset, 0);
                        MenuFormatStringArgument replacementFormat{};
                        S_004f21d0_ResetAndAssign(
                            reinterpret_cast<S_004f21d0*>(
                                replacementFormat.bytes),
                            nullptr, formatStringBytes);
                        reinterpret_cast<FormatCountWithArguments>(
                            FormatCountTextBuffer)(
                                replacementStringBytes, replacementFormat,
                                count, replacementMode);
                    }

                    Menu_ReplaceTextRange(
                        text, nullptr, tokenEnd, tokenString.field_0014,
                        replacementStringBytes, 0, 0xFFFFFFFF);
                    position = tokenEnd + replacementString.field_0014;
                }
            }
        }
        while (Menu_FindNextIndexedToken(&position, text, &tokenIndex));
    }

    SaveGame_DestroyLocalizedString(formatStringBytes, nullptr);
    SaveGame_DestroyLocalizedString(replacementStringBytes, nullptr);
    return static_cast<char>(
        SaveGame_DestroyLocalizedString(tokenStringBytes, nullptr));
}

HOOK(0x00735C70, C_008670f0_ExpandIndexedText, "auto");
