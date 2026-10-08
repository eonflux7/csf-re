#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl CopyStringRecordsBackward(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* destinationEnd)
{
    auto* sourceBegin = reinterpret_cast<std::uint8_t*>(first);
    auto* sourceCursor = reinterpret_cast<std::uint8_t*>(last);
    auto* destinationCursor =
        reinterpret_cast<std::uint8_t*>(destinationEnd);

    if (sourceBegin != sourceCursor)
    {
        do
        {
            CSF_AT(std::uint32_t, destinationCursor, -0x3C) =
                CSF_AT(std::uint32_t, sourceCursor, -0x3C);

            const std::uint32_t secondWord =
                CSF_AT(std::uint32_t, sourceCursor, -0x38);

            sourceCursor -= 0x3C;
            destinationCursor -= 0x3C;

            CSF_AT(std::uint32_t, destinationCursor, 0x04) = secondWord;
            CSF_AT(std::uint32_t, destinationCursor, 0x08) =
                CSF_AT(std::uint32_t, sourceCursor, 0x08);
            CSF_AT(std::uint32_t, destinationCursor, 0x0C) =
                CSF_AT(std::uint32_t, sourceCursor, 0x0C);
            CSF_AT(std::uint32_t, destinationCursor, 0x10) =
                CSF_AT(std::uint32_t, sourceCursor, 0x10);
            CSF_AT(std::uint32_t, destinationCursor, 0x14) =
                CSF_AT(std::uint32_t, sourceCursor, 0x14);

            AssignStringRange_488830(destinationCursor + 0x18, nullptr,
                              sourceCursor + 0x18, 0, UINT32_MAX);

            CSF_AT(std::uint32_t, destinationCursor, 0x34) =
                CSF_AT(std::uint32_t, sourceCursor, 0x34);
            CSF_AT(std::uint32_t, destinationCursor, 0x38) =
                CSF_AT(std::uint32_t, sourceCursor, 0x38);
        }
        while (sourceCursor != sourceBegin);
    }

    return reinterpret_cast<std::uint32_t*>(destinationCursor);
}

HOOK(0x00723610, CopyStringRecordsBackward, "auto");
