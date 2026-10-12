#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(S_004f21d0*, __fastcall, S_004f21d0_ResetAndAssign, 0x00488BE0, (S_004f21d0* self, void* , void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0, (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

CSF_FUNC(void*, __cdecl, CoreVariantValues_DescribeTypes, 0x00695EF0,
         (void* types, int first0, void* first4, int first8, int first12,
          int first16, int first20, unsigned int first24,
          int second0, void* second4, int second8, int second12,
          int second16, int second20, unsigned int second24));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static void CoreVariantValues_ReportTypes(
    void* types, void* firstSource, void* secondSource, bool useConstructor)
{
    alignas(8) std::uint8_t firstBytes[0x1C] = {};
    alignas(8) std::uint8_t secondBytes[0x1C] = {};

    if (useConstructor)
    {
        S_004f21d0_ResetAndAssign(
            reinterpret_cast<S_004f21d0*>(secondBytes), nullptr, secondSource);
        S_004f21d0_ResetAndAssign(
            reinterpret_cast<S_004f21d0*>(firstBytes), nullptr, firstSource);
    }
    else
    {
        CSF_AT(unsigned int, secondBytes, 0x18) = 7;
        CSF_AT(int, secondBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, secondBytes, 0x04) = 0;
        AssignStringRange_488830(secondBytes, nullptr, secondSource, 0, 0xFFFFFFFF);

        CSF_AT(int, firstBytes, 0x14) = 0;
        CSF_AT(unsigned int, firstBytes, 0x18) = 7;
        CSF_AT(std::uint16_t, firstBytes, 0x04) = 0;
        AssignStringRange_488830(firstBytes, nullptr, firstSource, 0, 0xFFFFFFFF);
    }

    CoreVariantValues_DescribeTypes(
        types,
        CSF_AT(int, firstBytes, 0x00),
        CSF_AT(void*, firstBytes, 0x04),
        CSF_AT(int, firstBytes, 0x08),
        CSF_AT(int, firstBytes, 0x0C),
        CSF_AT(int, firstBytes, 0x10),
        CSF_AT(int, firstBytes, 0x14),
        CSF_AT(unsigned int, firstBytes, 0x18),
        CSF_AT(int, secondBytes, 0x00),
        CSF_AT(void*, secondBytes, 0x04),
        CSF_AT(int, secondBytes, 0x08),
        CSF_AT(int, secondBytes, 0x0C),
        CSF_AT(int, secondBytes, 0x10),
        CSF_AT(int, secondBytes, 0x14),
        CSF_AT(unsigned int, secondBytes, 0x18));
}

static void CoreVariantValues_CopyNarrowText(char* destination, const char* source)
{
    const std::size_t length =
        std::strlen(source) > 0x400 ? 1023 : std::strlen(source);
    _strncpy(destination, source, length);
    destination[length] = 0;
}

static void* __cdecl CoreVariantValues_ReadArgument(
    void* output,
    unsigned int argumentIndex,
    void* types,
    int nameWord0,
    void* block,
    int nameWord8,
    int nameWord12,
    int nameWord16,
    int nameLength,
    unsigned int nameCapacity,
    void* contextString,
    std::uint8_t* valid,
    int* argumentCursor)
{
    // These seven stack words are one by-value 0x1C-byte string.
    alignas(8) std::uint8_t nameBytes[0x1C];
    CSF_AT(int, nameBytes, 0x00) = nameWord0;
    CSF_AT(void*, nameBytes, 0x04) = block;
    CSF_AT(int, nameBytes, 0x08) = nameWord8;
    CSF_AT(int, nameBytes, 0x0C) = nameWord12;
    CSF_AT(int, nameBytes, 0x10) = nameWord16;
    CSF_AT(int, nameBytes, 0x14) = nameLength;
    CSF_AT(unsigned int, nameBytes, 0x18) = nameCapacity;

    alignas(8) std::uint8_t destinationBytes[0x804] = {};
    char* destination = reinterpret_cast<char*>(destinationBytes);
    alignas(8) std::uint8_t resultBytes[0x800];

    if (*valid)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, types, 0x04);
        if (begin &&
            argumentIndex < static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, types, 0x08) - begin) >> 2))
        {
            unsigned int currentIndex = 0;
            do
            {
                const int type = CSF_AT(
                    int,
                    reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, types, 0x04)),
                    currentIndex * 4);

                void* argument = reinterpret_cast<void*>(
                    static_cast<std::uintptr_t>(
                        static_cast<std::uint32_t>(*argumentCursor)));

                switch (type)
                {
                case 'd':
                    if (CSF_AT(int, argument, 0) == 123456)
                    {
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, true);
                        *valid = 0;
                    }
                    else
                    {
                        *argumentCursor = static_cast<int>(
                            static_cast<std::uint32_t>(*argumentCursor) + 4);
                        const int value = CSF_AT(int, argument, 0);
                        if (currentIndex == argumentIndex)
                            CSF_AT(int, destinationBytes, 0) = value;
                    }
                    break;

                case 'f':
                    if (CSF_AT(int, argument, 0) == 123456)
                    {
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, true);
                        *valid = 0;
                    }
                    else
                    {
                        *argumentCursor = static_cast<int>(
                            static_cast<std::uint32_t>(*argumentCursor) + 8);
                        if (currentIndex == argumentIndex)
                            CSF_AT(float, destinationBytes, 0) =
                                static_cast<float>(CSF_AT(double, argument, 0));
                    }
                    break;

                case 's':
                    if (CSF_AT(int, argument, 0) == 123456)
                    {
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, true);
                        *valid = 0;
                    }
                    else
                    {
                        *argumentCursor = static_cast<int>(
                            static_cast<std::uint32_t>(*argumentCursor) + 4);
                        const char* text = CSF_AT(const char*, argument, 0);
                        if (currentIndex == argumentIndex)
                            CoreVariantValues_CopyNarrowText(destination, text);
                    }
                    break;

                case 'w':
                    if (CSF_AT(int, argument, 0) == 123456)
                    {
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, true);
                        *valid = 0;
                    }
                    else
                    {
                        *argumentCursor = static_cast<int>(
                            static_cast<std::uint32_t>(*argumentCursor) + 4);
                        std::uint16_t* text =
                            CSF_AT(std::uint16_t*, argument, 0);
                        if (currentIndex == argumentIndex)
                        {
                            const int length = GetWideTextLength(text) > 1024
                                ? 1023
                                : GetWideTextLength(text);
                            MenuLoading_CopyLocalizedText(
                                reinterpret_cast<std::uint16_t*>(destinationBytes),
                                text, length);
                            reinterpret_cast<std::uint16_t*>(
                                destinationBytes)[length] = 0;
                        }
                    }
                    break;

                default:
                    if (CSF_AT(int, argument, 0) == 123456)
                    {
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, false);
                        *valid = 0;
                    }
                    else
                    {
                        *argumentCursor = static_cast<int>(
                            static_cast<std::uint32_t>(*argumentCursor) + 4);
                        const char* text = CSF_AT(const char*, argument, 0);
                        if (currentIndex == argumentIndex)
                            CoreVariantValues_CopyNarrowText(destination, text);
                        CoreVariantValues_ReportTypes(
                            types, nameBytes, contextString, false);
                    }
                    break;
                }
                ++currentIndex;
            }
            while (currentIndex <= argumentIndex);

            const std::uint32_t finalBegin =
                CSF_AT(std::uint32_t, types, 0x04);
            const std::int32_t count = finalBegin
                ? static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, types, 0x08) - finalBegin) >> 2
                : 0;
            if (argumentIndex == static_cast<std::uint32_t>(count) - 1 &&
                CSF_AT(int, reinterpret_cast<void*>(
                    static_cast<std::uintptr_t>(
                        static_cast<std::uint32_t>(*argumentCursor))), 0) != 123456)
            {
                CoreVariantValues_ReportTypes(
                    types, nameBytes, contextString, false);
                *valid = 0;
            }
        }
        else
        {
            const char* emptyText =
                reinterpret_cast<const char*>(&g_byte_82E65C);
            std::size_t index = 0;
            char character;
            do
            {
                character = emptyText[index];
                destination[index++] = character;
            }
            while (character);
            *valid = 0;
        }
    }
    else
    {
        CSF_AT(int, destinationBytes, 0) = 0;
    }

    std::memcpy(resultBytes, destinationBytes, 0x800);
    if (CSF_AT(unsigned int, nameBytes, 0x18) >= 8 &&
        CSF_AT(void*, nameBytes, 0x04))
    {
        Mem_FreeDebug(CSF_AT(void*, nameBytes, 0x04));
    }
    std::memcpy(output, resultBytes, 0x800);
    return output;
}

HOOK(0x00696000, CoreVariantValues_ReadArgument, "auto");
