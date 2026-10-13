#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __cdecl, CoreVariantValues_CopyWideStringBounded, 0x006956C0, (int destBase, uint16_t* srcStr, int maxCount));
CSF_FUNC(S_004f21d0*, __cdecl, CoreVariantValues_FormatWideString, 0x006966E0, (S_004f21d0* destination, char sourceTag, void** sourceStorage, int sourceWord8, int sourceWord12, int sourceWord16, int sourceLength, unsigned int sourceCapacity, int* arguments));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));

static int __cdecl CoreVariantValues_FormatScore(
    int destination, int maxCount, S_004f21d0* pattern, ...)
{
    __builtin_va_list arguments;
    __builtin_va_start(arguments, pattern);
    int argumentCursor = reinterpret_cast<int>(arguments);

    alignas(8) uint8_t patternBytes[0x1C];
    alignas(8) uint8_t resultBytes[0x1C];
    alignas(8) uint8_t copyBytes[0x1C];

    auto& patternText = *reinterpret_cast<S_004f21d0*>(patternBytes);
    auto& resultText = *reinterpret_cast<S_004f21d0*>(resultBytes);
    auto& copiedPattern = *reinterpret_cast<S_004f21d0*>(copyBytes);

    S_004f21d0_SetPattern(
        &patternText, nullptr, reinterpret_cast<uint8_t*>(pattern));

    resultText.field_0018 = 7;
    resultText.field_0014 = 0;
    resultText.field_0004 = 0;

    copiedPattern.field_0018 = 7;
    copiedPattern.field_0014 = 0;
    copiedPattern.field_0004 = 0;

    AssignStringRange_488830(
        &copiedPattern, nullptr, &patternText, 0, 0xFFFFFFFF);

    CoreVariantValues_FormatWideString(
        &resultText,
        0,
        CSF_AT(void**, &copiedPattern.field_0004, 0),
        CSF_AT(int, copyBytes, 0x8),
        CSF_AT(int, copyBytes, 0xC),
        CSF_AT(int, copyBytes, 0x10),
        static_cast<int>(copiedPattern.field_0014),
        copiedPattern.field_0018,
        &argumentCursor);

    const unsigned int resultCapacity = resultText.field_0018;
    uint16_t* resultCharacters;
    if (resultCapacity < 8)
        resultCharacters = &resultText.field_0004;
    else
        resultCharacters = CSF_AT(uint16_t*, &resultText.field_0004, 0);

    CoreVariantValues_CopyWideStringBounded(
        destination, resultCharacters, maxCount);

    if (resultCapacity >= 8) {
        void* block = CSF_AT(void*, &resultText.field_0004, 0);
        if (block)
            Mem_FreeDebug(block);
    }

    resultText.field_0018 = 7;
    resultText.field_0014 = 0;
    resultText.field_0004 = 0;

    if (patternText.field_0018 >= 8) {
        void* block = CSF_AT(void*, &patternText.field_0004, 0);
        if (block)
            Mem_FreeDebug(block);
    }

    __builtin_va_end(arguments);
    return destination;
}

HOOK(0x006969C0, CoreVariantValues_FormatScore, "auto");
