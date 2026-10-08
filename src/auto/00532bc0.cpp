#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MenuEula_CompareWideCharacters, 0x005157F0,
         (std::uint16_t* left, std::uint16_t* right, int count));

static int __fastcall MenuEula_FindWideSubstring(
    void* self, void* /*edx*/, std::uint16_t* pattern,
    std::uint64_t packedSearch)
{
    const auto start = static_cast<std::uint32_t>(packedSearch);
    const auto count = static_cast<std::uint32_t>(packedSearch >> 32);

    if (count == 0 && start <= CSF_AT(std::uint32_t, self, 0x14))
        return static_cast<std::int32_t>(start);

    const std::uint32_t length = CSF_AT(std::uint32_t, self, 0x14);
    if (start >= length)
        return -1;

    const std::uint32_t available = length - start;
    if (count > available)
        return -1;

    std::uint32_t remaining = available + 1u - count;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x18);
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 4;
    const auto storageAddress =
        reinterpret_cast<std::uintptr_t>(storage);

    const std::uint32_t bufferAddress =
        capacity >= 8
            ? CSF_AT(std::uint32_t, storage, 0)
            : static_cast<std::uint32_t>(storageAddress);

    std::uint32_t searchAddress = bufferAddress + start * 2u;

    for (;;)
    {
        if (remaining == 0)
            return -1;

        std::uint32_t candidateAddress = searchAddress;
        std::uint32_t scanRemaining = remaining;
        const std::uint16_t firstCharacter = *pattern;

        while (*reinterpret_cast<std::uint16_t*>(candidateAddress)
               != firstCharacter)
        {
            candidateAddress += 2u;
            if (--scanRemaining == 0)
                return -1;
        }

        if (candidateAddress == 0)
            return -1;

        if (MenuEula_CompareWideCharacters(
                reinterpret_cast<std::uint16_t*>(candidateAddress),
                pattern, static_cast<std::int32_t>(count)) == 0)
        {
            const std::uint32_t baseAddress =
                capacity >= 8
                    ? CSF_AT(std::uint32_t, storage, 0)
                    : static_cast<std::uint32_t>(storageAddress);

            return static_cast<std::int32_t>(
                       candidateAddress - baseAddress) >> 1;
        }

        const std::int32_t skipped =
            static_cast<std::int32_t>(candidateAddress - searchAddress) >> 1;
        remaining += 0xFFFFFFFFu - static_cast<std::uint32_t>(skipped);
        searchAddress = candidateAddress + 2u;
    }
}

HOOK(0x00532BC0, MenuEula_FindWideSubstring, "auto");
