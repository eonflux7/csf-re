#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __cdecl CopyRecordPayloadsBackward(
    int sourceBeginAddress, int sourceEndAddress, int destinationEndAddress)
{
    const std::uint32_t sourceBegin =
        static_cast<std::uint32_t>(sourceBeginAddress);
    std::uint32_t source = static_cast<std::uint32_t>(sourceEndAddress);
    std::uint32_t destination =
        static_cast<std::uint32_t>(destinationEndAddress);

    if (source != sourceBegin)
    {
        const std::uint32_t addressDelta = source - destination;
        do
        {
            const std::uint32_t firstWord = CSF_AT(
                std::uint32_t,
                reinterpret_cast<void*>(addressDelta + destination), -8);

            destination -= 12u;
            source -= 12u;

            CSF_AT(std::uint32_t, reinterpret_cast<void*>(destination), 4) =
                firstWord;
            const std::uint32_t secondWord =
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(source), 8);
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(destination), 8) =
                secondWord;
        } while (source != sourceBegin);
    }

    return static_cast<int>(destination);
}

HOOK(0x0044ADE0, CopyRecordPayloadsBackward, "auto");
