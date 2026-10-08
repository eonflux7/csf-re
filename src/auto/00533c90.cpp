#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, _memchr, 0x007B0510,
         (const void* buffer, int character, std::size_t count));

static int __fastcall MenuMpPause_FindFirstCharacterNotInSet(
    void* self, void* /*edx*/, void* characterSet,
    std::uint32_t position, std::size_t setLength)
{
    const std::uint32_t length = CSF_AT(std::uint32_t, self, 0x14);
    if (position >= length)
        return -1;

    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x18);
    void* storage = static_cast<std::uint8_t*>(self) + 4;

    char* endBase = capacity < 0x10
        ? static_cast<char*>(storage)
        : CSF_AT(char*, storage, 0);
    const std::uintptr_t end =
        reinterpret_cast<std::uintptr_t>(endBase) + length;

    char* startBase = capacity < 0x10
        ? static_cast<char*>(storage)
        : CSF_AT(char*, storage, 0);
    std::uintptr_t cursor =
        reinterpret_cast<std::uintptr_t>(startBase) + position;

    if (cursor >= end)
        return -1;

    for (;;)
    {
        const int character =
            static_cast<int>(*reinterpret_cast<const std::int8_t*>(cursor));

        if (_memchr(characterSet, character, setLength) == nullptr)
        {
            char* currentBase = static_cast<char*>(storage);
            if (CSF_AT(std::uint32_t, self, 0x18) >= 0x10)
                currentBase = CSF_AT(char*, storage, 0);

            return static_cast<std::int32_t>(
                cursor - reinterpret_cast<std::uintptr_t>(currentBase));
        }

        ++cursor;
        if (cursor >= end)
            return -1;
    }
}

HOOK(0x00533C90, MenuMpPause_FindFirstCharacterNotInSet, "auto");
