#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, GameMemchr, 0x007B0510,
         (const void* buffer, int character, std::size_t count));

static int __fastcall MenuMpPause_FindSubstring(
    void* self, void* /*edx*/, char* pattern, std::uint64_t searchArguments)
{
    const std::uint32_t position =
        static_cast<std::uint32_t>(searchArguments);
    const std::uint32_t patternLength =
        static_cast<std::uint32_t>(searchArguments >> 32);

    if (patternLength == 0 && position <= CSF_AT(std::uint32_t, self, 0x14))
        return static_cast<std::int32_t>(position);

    const std::uint32_t length = CSF_AT(std::uint32_t, self, 0x14);
    if (position >= length)
        return -1;

    const std::uint32_t remaining = length - position;
    if (patternLength > remaining)
        return -1;

    std::uint32_t candidateCount = remaining + 1u - patternLength;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x18);
    void* storage = static_cast<std::uint8_t*>(self) + 4;
    char* data = capacity < 0x10
        ? static_cast<char*>(storage)
        : CSF_AT(char*, storage, 0);

    std::uintptr_t searchStart =
        reinterpret_cast<std::uintptr_t>(data) + position;

    char* candidate = static_cast<char*>(GameMemchr(
        reinterpret_cast<const void*>(searchStart),
        static_cast<int>(*reinterpret_cast<const std::int8_t*>(pattern)),
        candidateCount));

    while (candidate != nullptr)
    {
        std::uint32_t matched = 0;
        while (matched < patternLength)
        {
            const std::uint8_t candidateByte =
                reinterpret_cast<const std::uint8_t*>(candidate)[matched];
            const std::uint8_t patternByte =
                reinterpret_cast<const std::uint8_t*>(pattern)[matched];
            if (candidateByte != patternByte)
                break;
            ++matched;
        }

        if (matched == patternLength)
        {
            char* currentData = static_cast<char*>(storage);
            if (CSF_AT(std::uint32_t, self, 0x18) >= 0x10)
                currentData = CSF_AT(char*, storage, 0);

            return static_cast<std::int32_t>(
                reinterpret_cast<std::uintptr_t>(candidate)
                - reinterpret_cast<std::uintptr_t>(currentData));
        }

        const std::uintptr_t candidateAddress =
            reinterpret_cast<std::uintptr_t>(candidate);
        candidateCount += static_cast<std::uint32_t>(
            searchStart - candidateAddress - 1u);
        searchStart = candidateAddress + 1u;

        candidate = static_cast<char*>(GameMemchr(
            reinterpret_cast<const void*>(searchStart),
            static_cast<int>(*reinterpret_cast<const std::int8_t*>(pattern)),
            candidateCount));
    }

    return -1;
}

HOOK(0x00533BC0, MenuMpPause_FindSubstring, "auto");
