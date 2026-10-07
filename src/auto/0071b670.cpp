#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl CopyStringPairRecordsBackward(
    std::uint32_t* sourceBegin,
    std::uint32_t* sourceEnd,
    std::uint32_t* destinationEnd)
{
    if (sourceBegin == sourceEnd)
        return destinationEnd;

    auto* source = sourceEnd;
    auto* destination = destinationEnd;

    do
    {
        source -= 16;
        destination -= 16;

        AssignStringSubstring(destination, nullptr, source, 0, UINT32_MAX);
        AssignStringSubstring(
            reinterpret_cast<std::uint8_t*>(destination) + 0x1C,
            nullptr,
            reinterpret_cast<std::uint8_t*>(source) + 0x1C,
            0, UINT32_MAX);

        CSF_AT(std::uint32_t, destination, 0x38) =
            CSF_AT(std::uint32_t, source, 0x38);
        CSF_AT(std::uint32_t, destination, 0x3C) =
            CSF_AT(std::uint32_t, source, 0x3C);
    }
    while (source != sourceBegin);

    return destination;
}

HOOK(0x0071B670, CopyStringPairRecordsBackward, "auto");
