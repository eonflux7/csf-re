#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0, (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(void*, __cdecl, CoreVariantValues_FormatPattern, 0x006966E0, (void* a1, char a2, void** a3, int a4, int a5, int a6, int a7, unsigned int a8, int* a9));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));

static int __cdecl CoreVariantValues_FormatScore(
    int destination, int capacity, S_004f21d0* pattern, ...)
{
    __builtin_va_list arguments;
    __builtin_va_start(arguments, pattern);
    int argumentCursor = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(arguments));

    alignas(8) std::uint8_t patternBytes[0x1C] = {};
    alignas(8) std::uint8_t resultBytes[0x1C] = {};
    alignas(8) std::uint8_t copiedPatternBytes[0x1C] = {};

    FormatterSetPattern(
        patternBytes, nullptr, reinterpret_cast<const char*>(pattern));

    CSF_AT(std::uint32_t, resultBytes, 0x18) = 7;
    CSF_AT(int, resultBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, resultBytes, 0x04) = 0;

    CSF_AT(std::uint32_t, copiedPatternBytes, 0x18) = 7;
    CSF_AT(int, copiedPatternBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, copiedPatternBytes, 0x04) = 0;

    AssignStringRange_488830(
        copiedPatternBytes, nullptr, patternBytes, 0, 0xFFFFFFFF);

    CoreVariantValues_FormatPattern(
        resultBytes,
        CSF_AT(char, copiedPatternBytes, 0x00),
        CSF_AT(void**, copiedPatternBytes, 0x04),
        CSF_AT(int, copiedPatternBytes, 0x08),
        CSF_AT(int, copiedPatternBytes, 0x0C),
        CSF_AT(int, copiedPatternBytes, 0x10),
        CSF_AT(int, copiedPatternBytes, 0x14),
        CSF_AT(unsigned int, copiedPatternBytes, 0x18),
        &argumentCursor);

    const std::uint32_t resultCapacity =
        CSF_AT(std::uint32_t, resultBytes, 0x18);
    const uint16_t* text = CSF_AT(uint16_t*, resultBytes, 0x04);
    if (resultCapacity < 8)
        text = reinterpret_cast<const uint16_t*>(resultBytes + 0x04);

    MenuLoading_CopyLocalizedText(
        reinterpret_cast<uint16_t*>(destination), text, capacity);

    if (resultCapacity >= 8 && CSF_AT(void*, resultBytes, 0x04))
        Mem_FreeDebug(CSF_AT(void*, resultBytes, 0x04));

    CSF_AT(std::uint32_t, resultBytes, 0x18) = 7;
    CSF_AT(int, resultBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, resultBytes, 0x04) = 0;

    if (CSF_AT(std::uint32_t, patternBytes, 0x18) >= 8 &&
        CSF_AT(void*, patternBytes, 0x04))
        Mem_FreeDebug(CSF_AT(void*, patternBytes, 0x04));

    __builtin_va_end(arguments);
    return destination;
}

HOOK(0x006969C0, CoreVariantValues_FormatScore, "auto");
