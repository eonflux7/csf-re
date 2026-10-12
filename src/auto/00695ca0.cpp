#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(double, g_DecimalPowerBase, 0x0084CAE0);

static const auto kPacketWideStringVtable = CSF_VTBL(0x004F21D0);

static char __cdecl CoreVariantValues_ParseUnsignedInteger(
    std::int16_t** cursor, std::uint32_t* value)
{
    alignas(8) std::uint8_t frameBytes[0x24];
    legacy::wstring& text =
        *reinterpret_cast<legacy::wstring*>(frameBytes);

    CSF_AT(std::uint32_t, frameBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, frameBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, frameBytes, 0x4) = 0;

    const auto inlineAddress = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(frameBytes + 0x4));

    EraseWideStringRange(
        &text, nullptr, reinterpret_cast<int*>(frameBytes + 0x1C),
        inlineAddress, inlineAddress);

    std::int16_t character = **cursor;
    char success = 0;

    if (character != 0)
    {
        ++*cursor;
        if (static_cast<std::uint16_t>(character) >= 0x30u)
        {
            while (static_cast<std::uint16_t>(character) <= 0x39u)
            {
                auto* buffer = CSF_AT(std::int16_t*, frameBytes, 0x4);
                if (CSF_AT(std::uint32_t, frameBytes, 0x18) < 8)
                    buffer = reinterpret_cast<std::int16_t*>(frameBytes + 0x4);

                auto* end = buffer + CSF_AT(std::uint32_t, frameBytes, 0x14);
                const unsigned int position =
                    end ? static_cast<unsigned int>(end - buffer) : 0;

                InsertWideCharacters(&text, nullptr, position, 1u, character);

                character = **cursor;
                if (character == 0)
                    break;

                ++*cursor;
                if (static_cast<std::uint16_t>(character) < 0x30u)
                    break;
            }
        }

        if (character != 0)
            --*cursor;
    }

    const std::uint32_t digitCount =
        CSF_AT(std::uint32_t, frameBytes, 0x14);

    if (digitCount != 0)
    {
        success = 1;
        *value = 0;

        std::uint32_t exponent = 0;
        std::uint32_t byteOffset = 2 * digitCount;

        for (;;)
        {
            const double power =
                std::pow(g_DecimalPowerBase, static_cast<double>(exponent));

            auto* buffer = CSF_AT(std::uint8_t*, frameBytes, 0x4);
            if (CSF_AT(std::uint32_t, frameBytes, 0x18) < 8)
                buffer = frameBytes + 0x4;

            const int digit =
                *reinterpret_cast<std::uint16_t*>(buffer + byteOffset - 2) - 48;
            const auto contribution = static_cast<std::int64_t>(digit * power);
            const std::uint32_t accumulated =
                *value + static_cast<std::uint32_t>(contribution);

            ++exponent;
            byteOffset -= 2;
            const bool more =
                exponent < CSF_AT(std::uint32_t, frameBytes, 0x14);
            *value = accumulated;

            if (!more)
                break;
        }
    }

    if (CSF_AT(std::uint32_t, frameBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, frameBytes, 0x4);
        if (block)
            Mem_FreeDebug(block);
    }

    return success;
}

HOOK(0x00695CA0, CoreVariantValues_ParseUnsignedInteger, "auto");
