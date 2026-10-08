#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60,
         (void* self, void* edx, const void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl MenuExtras_CopyEntriesBackward(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* destinationEnd)
{
    if (first == last)
        return destinationEnd;

    auto* source = reinterpret_cast<std::uint8_t*>(last);
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationEnd);

    do
    {
        const std::uint32_t firstWord =
            CSF_AT(std::uint32_t, source - 0x44, 0);

        source -= 0x44;
        destination -= 0x44;
        CSF_AT(std::uint32_t, destination, 0) = firstWord;

        MenuExtras_AssignStringRange(
            destination + 0x04, nullptr, source + 0x04,
            0, 0xFFFFFFFFu);

        CSF_AT(std::uint32_t, destination, 0x20) =
            CSF_AT(std::uint32_t, source, 0x20);
        CSF_AT(std::uint32_t, destination, 0x24) =
            CSF_AT(std::uint32_t, source, 0x24);

        MenuExtras_AssignStringRange(
            destination + 0x28, nullptr, source + 0x28,
            0, 0xFFFFFFFFu);
    }
    while (source != reinterpret_cast<std::uint8_t*>(first));

    return reinterpret_cast<std::uint32_t*>(destination);
}

HOOK(0x00546F10, MenuExtras_CopyEntriesBackward, "auto");
