#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, CoreVariantValues_FormatArgument, 0x00695B80, (void* a1, int a2, void* a3));
CSF_FUNC(char, __cdecl, CoreVariantValues_ParseNextToken, 0x00695DE0, (void** a1, void*** a2, int16_t* a3, bool* a4));
CSF_FUNC(void*, __cdecl, CoreVariantValues_ExtractArgument, 0x00696000, (void* a1, unsigned int arg4, int a3, char a4, void* Block, int a6, int a7, int a8, int a9, unsigned int a10, void* a2, uint8_t* a12, int* a13));
CSF_FUNC(char, __fastcall, CoreVariantValues_ResizeArgumentTypes, 0x00696670, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(void*, __fastcall, CoreVariantValues_AppendString, 0x00696F00, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));

static S_004f21d0* __cdecl CoreVariantValues_FormatWideString(
    S_004f21d0* destination,
    char sourceTag,
    void** sourceStorage,
    int sourceWord8,
    int sourceWord12,
    int sourceWord16,
    int sourceLength,
    unsigned int sourceCapacity,
    int* arguments)
{
    // The packet labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x004F21D0);

    // Reconstitute the by-value string's contiguous argument storage.
    alignas(8) std::uint8_t sourceBytes[0x1C]{};
    CSF_AT(char, sourceBytes, 0) = sourceTag;
    CSF_AT(void**, sourceBytes, 0x4) = sourceStorage;
    CSF_AT(int, sourceBytes, 0x8) = sourceWord8;
    CSF_AT(int, sourceBytes, 0xC) = sourceWord12;
    CSF_AT(int, sourceBytes, 0x10) = sourceWord16;
    CSF_AT(int, sourceBytes, 0x14) = sourceLength;
    CSF_AT(unsigned int, sourceBytes, 0x18) = sourceCapacity;

    alignas(8) std::uint8_t extractedBytes[0x800];
    alignas(8) std::uint8_t argumentBytes[0x800];
    alignas(8) std::uint8_t formattedBytes[0x1C];
    alignas(8) std::uint8_t fallbackBytes[0x1C];
    alignas(8) std::uint8_t appendBytes[0x1C];
    alignas(8) std::uint8_t argumentCursorBytes[0x4];
    alignas(8) std::uint8_t typeTableBytes[0x10];
    alignas(8) std::uint8_t tokenBytes[0x8];
    alignas(8) std::uint8_t scanBytes[0x4];
    alignas(8) std::uint8_t tokenStateBytes[0xF];

    auto& typeTable = *reinterpret_cast<S_006959b0*>(typeTableBytes);
    bool finished = false;

    const std::uint32_t byteCount =
        2 * static_cast<std::uint32_t>(sourceLength);
    auto* copiedText = static_cast<std::uint8_t*>(Mem_Alloc(byteCount + 2));
    const void* sourceText = sourceStorage;
    if (sourceCapacity < 8)
        sourceText = sourceBytes + 0x4;
    std::memcpy(copiedText, sourceText, byteCount);
    CSF_AT(std::uint16_t, copiedText, byteCount) = 0;

    CSF_AT(void*, scanBytes, 0) = copiedText;
    CSF_AT(void*, tokenBytes, 0x4) = copiedText;
    CSF_AT(int, tokenStateBytes, 0) = 0;
    CSF_AT(int, tokenStateBytes, 0x4) = 0;
    typeTable.field_0004 = 0;
    typeTable.field_0008 = 0;
    CSF_AT(int, typeTableBytes, 0xC) = 0;

    std::uint32_t typeStorage = 0;
    do
    {
        if (CoreVariantValues_ParseNextToken(
                reinterpret_cast<void**>(scanBytes),
                reinterpret_cast<void***>(tokenStateBytes),
                reinterpret_cast<int16_t*>(tokenBytes), &finished))
        {
            std::uint32_t count = 0;
            if (typeStorage)
            {
                count = static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        typeTable.field_0008 - typeStorage) >> 2);
            }

            const int index = CSF_AT(int, tokenStateBytes, 0);
            const unsigned int required = static_cast<unsigned int>(index) + 1;
            if (count < required)
            {
                CoreVariantValues_ResizeArgumentTypes(
                    typeTableBytes, nullptr, required, 0);
                typeStorage = typeTable.field_0004;
            }
            reinterpret_cast<int*>(typeStorage)[index] =
                CSF_AT(int, tokenStateBytes, 0x4);
        }
    } while (!finished);

    CSF_AT(unsigned int, appendBytes, 0x18) = 7;
    CSF_AT(int, appendBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, appendBytes, 0x4) = 0;
    finished = false;

    const std::uint32_t capacity = destination->field_0018;
    auto* first = &destination->field_0004;
    auto* endStorage = capacity < 8
        ? first
        : *reinterpret_cast<std::uint16_t**>(first);
    const auto last = reinterpret_cast<std::uint32_t>(
        endStorage + destination->field_0014);
    if (capacity >= 8)
        first = *reinterpret_cast<std::uint16_t**>(first);

    int erasedPosition;
    EraseWideStringRange(
        reinterpret_cast<legacy::wstring*>(destination), nullptr,
        &erasedPosition, reinterpret_cast<std::uint32_t>(first), last);

    CSF_AT(void*, scanBytes, 0) = CSF_AT(void*, tokenBytes, 0x4);
    std::uint8_t argumentsValid = 1;

    do
    {
        const bool isArgument = CoreVariantValues_ParseNextToken(
            reinterpret_cast<void**>(scanBytes),
            reinterpret_cast<void***>(tokenStateBytes),
            reinterpret_cast<int16_t*>(tokenBytes), &finished) != 0;

        if (!isArgument)
        {
            if (finished)
                break;
            SaveSlots_ApplyIndex(
                destination, nullptr, CSF_AT(int16_t, tokenBytes, 0));
            continue;
        }

        CSF_AT(int, argumentCursorBytes, 0) = *arguments;

        alignas(8) std::uint8_t sourceCopyBytes[0x1C]{};
        CSF_AT(unsigned int, sourceCopyBytes, 0x18) = 7;
        CSF_AT(int, sourceCopyBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, sourceCopyBytes, 0x4) = 0;
        AssignStringRange_488830(
            sourceCopyBytes, nullptr, sourceBytes, 0, 0xFFFFFFFF);

        void* extracted = CoreVariantValues_ExtractArgument(
            extractedBytes,
            CSF_AT(unsigned int, tokenStateBytes, 0),
            reinterpret_cast<int>(typeTableBytes),
            CSF_AT(char, sourceCopyBytes, 0),
            CSF_AT(void*, sourceCopyBytes, 0x4),
            CSF_AT(int, sourceCopyBytes, 0x8),
            CSF_AT(int, sourceCopyBytes, 0xC),
            CSF_AT(int, sourceCopyBytes, 0x10),
            CSF_AT(int, sourceCopyBytes, 0x14),
            CSF_AT(unsigned int, sourceCopyBytes, 0x18),
            destination, &argumentsValid,
            reinterpret_cast<int*>(argumentCursorBytes));
        std::memcpy(argumentBytes, extracted, sizeof(argumentBytes));

        void* formatted;
        if (argumentsValid)
        {
            formatted = reinterpret_cast<void*>(
                CoreVariantValues_FormatArgument(
                    formattedBytes,
                    CSF_AT(int, tokenStateBytes, 0x4), argumentBytes));
        }
        else
        {
            formatted = reinterpret_cast<void*>(
                FormatterSetPattern(
                    fallbackBytes, nullptr,
                    reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0085E6B4))));
        }

        AssignStringRange_488830(
            appendBytes, nullptr, formatted, 0, 0xFFFFFFFF);

        if (!argumentsValid)
        {
            if (CSF_AT(unsigned int, fallbackBytes, 0x18) >= 8 &&
                CSF_AT(void*, fallbackBytes, 0x4))
            {
                Mem_FreeDebug(CSF_AT(void*, fallbackBytes, 0x4));
            }
            CSF_AT(unsigned int, fallbackBytes, 0x18) = 7;
            CSF_AT(int, fallbackBytes, 0x14) = 0;
            CSF_AT(std::uint16_t, fallbackBytes, 0x4) = 0;
        }
        else
        {
            if (CSF_AT(unsigned int, formattedBytes, 0x18) >= 8 &&
                CSF_AT(void*, formattedBytes, 0x4))
            {
                Mem_FreeDebug(CSF_AT(void*, formattedBytes, 0x4));
            }
            CSF_AT(unsigned int, formattedBytes, 0x18) = 7;
            CSF_AT(int, formattedBytes, 0x14) = 0;
            CSF_AT(std::uint16_t, formattedBytes, 0x4) = 0;
        }

        CoreVariantValues_AppendString(
            destination, nullptr, reinterpret_cast<uint32_t*>(appendBytes));
    } while (!finished);

    if (CSF_AT(void*, tokenBytes, 0x4))
        Mem_Free(CSF_AT(void*, tokenBytes, 0x4));

    if (CSF_AT(unsigned int, appendBytes, 0x18) >= 8 &&
        CSF_AT(void*, appendBytes, 0x4))
    {
        Mem_FreeDebug(CSF_AT(void*, appendBytes, 0x4));
    }
    CSF_AT(unsigned int, appendBytes, 0x18) = 7;
    CSF_AT(int, appendBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, appendBytes, 0x4) = 0;

    if (typeTable.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(typeTable.field_0004));
    if (sourceCapacity >= 8 && sourceStorage)
        Mem_FreeDebug(sourceStorage);

    return destination;
}

HOOK(0x006966E0, CoreVariantValues_FormatWideString, "auto");
