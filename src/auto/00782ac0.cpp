#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static void* __cdecl CopyStringRecordsBackward(
    const void* sourceBegin,
    const void* sourceEnd,
    C_0082d410* destinationEnd,
    int /*unused*/)
{
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationEnd);
    auto* source = static_cast<const std::uint8_t*>(sourceEnd);

    if (sourceBegin != sourceEnd)
    {
        auto* destinationCursor = destination + 108;
        auto* sourceCursor = source + 108;

        do
        {
            source -= 348;
            destination -= 348;
            destinationCursor -= 348;
            sourceCursor -= 348;

            AssignStringSubstring(destination, nullptr, source, 0, -1);

            std::memcpy(destinationCursor - 80, sourceCursor - 80, 0x50u);
            std::memcpy(destinationCursor, sourceCursor, 0x50u);
            std::memcpy(destinationCursor + 80, sourceCursor + 80, 0x50u);
            std::memcpy(destinationCursor + 160, sourceCursor + 160, 0x50u);
        }
        while (source != sourceBegin);
    }

    return destination;
}

HOOK(0x00782AC0, CopyStringRecordsBackward, "auto");
