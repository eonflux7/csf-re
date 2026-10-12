#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static int __cdecl ScreenEffects_CopyRecordsBackward(
    int first, int last, int destinationEnd, int /*unused*/)
{
    auto* sourceEnd = reinterpret_cast<std::uint8_t*>(last);
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationEnd);

    if (first != last)
    {
        auto* sourceTail = sourceEnd + 32;
        auto* destinationTail = destination + 32;
        do
        {
            const std::uint32_t value = CSF_AT(std::uint32_t, sourceEnd, -44);
            sourceEnd -= 44;
            destination -= 44;
            sourceTail -= 44;
            destinationTail -= 44;

            CSF_AT(std::uint32_t, destination, 0) = value;
            AssignStringSubstring(destinationTail - 28, nullptr,
                                  sourceTail - 28, 0, -1);
            CSF_AT(std::uint32_t, destinationTail, 0) =
                CSF_AT(std::uint32_t, sourceTail, 0);
            CSF_AT(std::uint32_t, destinationTail, 4) =
                CSF_AT(std::uint32_t, sourceTail, 4);
            CSF_AT(std::uint32_t, destinationTail, 8) =
                CSF_AT(std::uint32_t, sourceTail, 8);
        }
        while (sourceEnd != reinterpret_cast<std::uint8_t*>(first));
    }

    return reinterpret_cast<int>(destination);
}

HOOK(0x006F8860, ScreenEffects_CopyRecordsBackward, "auto");
