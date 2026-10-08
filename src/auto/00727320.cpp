#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl CopyStringRecordRange(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* destination)
{
    if (first == last)
        return destination;

    do
    {
        CSF_AT(std::uint32_t, destination, 0x00) =
            CSF_AT(std::uint32_t, first, 0x00);

        AssignStringSubstring(
            reinterpret_cast<std::uint8_t*>(destination) + 0x04,
            nullptr,
            reinterpret_cast<std::uint8_t*>(first) + 0x04,
            0,
            0xFFFFFFFFu);

        CSF_AT(std::uint32_t, destination, 0x20) =
            CSF_AT(std::uint32_t, first, 0x20);

        first += 9;
        destination += 9;
    }
    while (first != last);

    return destination;
}

HOOK(0x00727320, CopyStringRecordRange, "auto");
