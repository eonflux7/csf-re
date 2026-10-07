#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl CopyStringRecordsBackward(
    const std::uint8_t* sourceBegin,
    const std::uint8_t* sourceEnd,
    std::uint32_t* destinationEnd)
{
    if (sourceBegin == sourceEnd)
        return destinationEnd;

    do
    {
        const std::uint32_t firstValue =
            CSF_AT(std::uint32_t, sourceEnd, -0x24);

        sourceEnd -= 0x24;
        destinationEnd -= 9;

        CSF_AT(std::uint32_t, destinationEnd, 0) = firstValue;

        AssignStringSubstring(
            reinterpret_cast<std::uint8_t*>(destinationEnd) + 4,
            nullptr,
            sourceEnd + 4,
            0,
            0xFFFFFFFFu);

        CSF_AT(std::uint32_t, destinationEnd, 0x20) =
            CSF_AT(std::uint32_t, sourceEnd, 0x20);
    }
    while (sourceEnd != sourceBegin);

    return destinationEnd;
}

HOOK(0x007273C0, CopyStringRecordsBackward, "auto");
