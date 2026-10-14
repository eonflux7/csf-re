#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(S_004f21d0*, __fastcall, S_004f21d0_ResetAndAssign, 0x00488BE0, (S_004f21d0* self, void* , void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(void, __cdecl, CoreVariantValues_CopyWideStringBounded, 0x006956C0, (int destBase, uint16_t* srcStr, int maxCount));
CSF_FUNC(void*, __cdecl, CoreVariantValues_FormatTypesAndReleaseStrings, 0x00695EF0, (void* typeCodes, int , void* firstBuffer, int , int , int , int , unsigned int firstCapacity, int , void* secondBuffer, int , int , int , int , unsigned int secondCapacity));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static __forceinline void CoreVariantValues_FormatArgumentTypes(
    void* typeCodes, void* firstSource, void* secondSource, bool useReset)
{
    alignas(8) uint8_t firstBytes[0x1C];
    alignas(8) uint8_t secondBytes[0x1C];

    if (useReset)
    {
        S_004f21d0_ResetAndAssign(
            reinterpret_cast<S_004f21d0*>(secondBytes), nullptr, secondSource);
        S_004f21d0_ResetAndAssign(
            reinterpret_cast<S_004f21d0*>(firstBytes), nullptr, firstSource);
    }
    else
    {
        auto& second = *reinterpret_cast<S_004f21d0*>(secondBytes);
        second.field_0018 = 7;
        second.field_0014 = 0;
        second.field_0004 = 0;
        AssignStringRange_488830(secondBytes, nullptr, secondSource, 0, 0xFFFFFFFF);

        auto& first = *reinterpret_cast<S_004f21d0*>(firstBytes);
        first.field_0014 = 0;
        first.field_0018 = 7;
        first.field_0004 = 0;
        AssignStringRange_488830(firstBytes, nullptr, firstSource, 0, 0xFFFFFFFF);
    }

    CoreVariantValues_FormatTypesAndReleaseStrings(
        typeCodes,
        CSF_AT(int, firstBytes, 0),
        CSF_AT(void*, firstBytes, 4),
        CSF_AT(int, firstBytes, 8),
        CSF_AT(int, firstBytes, 12),
        CSF_AT(int, firstBytes, 16),
        CSF_AT(int, firstBytes, 20),
        CSF_AT(unsigned int, firstBytes, 24),
        CSF_AT(int, secondBytes, 0),
        CSF_AT(void*, secondBytes, 4),
        CSF_AT(int, secondBytes, 8),
        CSF_AT(int, secondBytes, 12),
        CSF_AT(int, secondBytes, 16),
        CSF_AT(int, secondBytes, 20),
        CSF_AT(unsigned int, secondBytes, 24));
}

static __forceinline void CoreVariantValues_CopyNarrowArgument(
    char* destination, const char* source)
{
    const size_t length =
        std::strlen(source) > 0x400 ? 1023 : std::strlen(source);
    _strncpy(destination, source, length);
    destination[length] = 0;
}

static uint32_t __cdecl CoreVariantValues_ReadArgument(
    hook::Regs* regs, const uint32_t* args)
{
    void* result = reinterpret_cast<void*>(args[0]);
    unsigned int selectedIndex = args[1];
    void* typeCodes = reinterpret_cast<void*>(args[2]);
    void* firstStringBytes = const_cast<uint32_t*>(args + 3);
    void* block = reinterpret_cast<void*>(args[4]);
    unsigned int capacity = args[9];
    void* secondString = reinterpret_cast<void*>(args[10]);
    auto* valid = reinterpret_cast<uint8_t*>(args[11]);
    auto* argumentCursor = reinterpret_cast<int*>(args[12]);

    alignas(8) uint8_t destinationBytes[0x804];
    char* destination = reinterpret_cast<char*>(destinationBytes);
    // The large-frame probe at entry pushes ECX at EBP-8. That word is
    // the final dword of Destination and both rep movsd copies include it.
    CSF_AT(uint32_t, destinationBytes, 0x7FC) = regs->ecx;

    if (!*valid)
    {
        CSF_AT(int, destinationBytes, 0) = 0;
    }
    else
    {
        const uint32_t begin = CSF_AT(uint32_t, typeCodes, 4);
        const int count = begin
            ? static_cast<int32_t>(CSF_AT(uint32_t, typeCodes, 8) - begin) >> 2
            : 0;

        if (!begin || selectedIndex >= static_cast<unsigned int>(count))
        {
            size_t index = 0;
            uint8_t character;
            do
            {
                character = (&g_byte_82E65C)[index];
                destinationBytes[index++] = character;
            } while (character);
            *valid = 0;
        }
        else
        {
            unsigned int index = 0;
            do
            {
                const int code = CSF_AT(
                    int, reinterpret_cast<void*>(CSF_AT(uint32_t, typeCodes, 4)),
                    index * 4);

                switch (code)
                {
                case 'd':
                case 'f':
                case 's':
                case 'w':
                {
                    void* current = reinterpret_cast<void*>(*argumentCursor);
                    if (CSF_AT(int, current, 0) == 123456)
                    {
                        CoreVariantValues_FormatArgumentTypes(
                            typeCodes, firstStringBytes, secondString, true);
                        *valid = 0;
                        break;
                    }

                    const uint32_t advanced =
                        static_cast<uint32_t>(*argumentCursor)
                        + (code == 'f' ? 8 : 4);
                    *argumentCursor = static_cast<int32_t>(advanced);
                    void* value = reinterpret_cast<void*>(
                        advanced - (code == 'f' ? 8 : 4));

                    if (index == selectedIndex)
                    {
                        switch (code)
                        {
                        case 'd':
                            CSF_AT(int, destinationBytes, 0) =
                                CSF_AT(int, value, 0);
                            break;
                        case 'f':
                            CSF_AT(float, destinationBytes, 0) =
                                static_cast<float>(CSF_AT(double, value, 0));
                            break;
                        case 's':
                            CoreVariantValues_CopyNarrowArgument(
                                destination, CSF_AT(const char*, value, 0));
                            break;
                        case 'w':
                        {
                            uint16_t* text = CSF_AT(uint16_t*, value, 0);
                            const int length =
                                StringFormat_GetWideTextLength(text) > 1024
                                ? 1023 : StringFormat_GetWideTextLength(text);
                            CoreVariantValues_CopyWideStringBounded(
                                reinterpret_cast<int>(destination), text, length);
                            CSF_AT(uint16_t, destinationBytes, 2 * length) = 0;
                            break;
                        }
                        }
                    }
                    break;
                }
                default:
                {
                    void* current = reinterpret_cast<void*>(*argumentCursor);
                    const bool sentinel =
                        CSF_AT(int, current, 0) == 123456;
                    if (!sentinel)
                    {
                        const uint32_t advanced =
                            static_cast<uint32_t>(*argumentCursor) + 4;
                        *argumentCursor = static_cast<int32_t>(advanced);
                        const char* text = CSF_AT(
                            const char*, reinterpret_cast<void*>(advanced - 4), 0);
                        if (index == selectedIndex)
                            CoreVariantValues_CopyNarrowArgument(destination, text);
                    }
                    CoreVariantValues_FormatArgumentTypes(
                        typeCodes, firstStringBytes, secondString, false);
                    if (sentinel)
                        *valid = 0;
                    break;
                }
                }
                ++index;
            } while (index <= selectedIndex);

            const uint32_t finalBegin = CSF_AT(uint32_t, typeCodes, 4);
            const int finalCount = finalBegin
                ? static_cast<int32_t>(
                    CSF_AT(uint32_t, typeCodes, 8) - finalBegin) >> 2
                : 0;

            if (selectedIndex == static_cast<unsigned int>(finalCount - 1)
                && CSF_AT(int, reinterpret_cast<void*>(*argumentCursor), 0)
                    != 123456)
            {
                CoreVariantValues_FormatArgumentTypes(
                    typeCodes, firstStringBytes, secondString, false);
                *valid = 0;
            }
        }
    }

    // 006963A8..006963B9 snapshot the whole result before releasing the
    // by-value string. Keep that staging copy, even if the free aliases inputs.
    alignas(8) uint8_t snapshotBytes[0x800];
    for (unsigned int offset = 0; offset < 0x800; offset += 4)
        CSF_AT(uint32_t, snapshotBytes, offset) =
            CSF_AT(uint32_t, destinationBytes, offset);
    if (capacity >= 8 && block)
        Mem_FreeDebug(block);

    // Explicit paired stores avoid introducing a CRT memory-copy call.
    for (unsigned int offset = 0; offset < 0x800; offset += 8)
    {
        CSF_AT(uint32_t, result, offset + 4) =
            CSF_AT(uint32_t, snapshotBytes, offset + 4);
        CSF_AT(uint32_t, result, offset) =
            CSF_AT(uint32_t, snapshotBytes, offset);
    }
    return reinterpret_cast<uint32_t>(result);
}

HOOK_REGS(0x00696000, CoreVariantValues_ReadArgument, 0, "auto");
