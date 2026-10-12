#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Walks a range of 0x3C-byte records backwards from sourceEnd down to rangeStart.
// For each record: re-assigns the string at the matching destination record (whole
// string, substring from 0, count -1), then copies the record's tail dwords
// (-4 .. +0x18, 8 dwords) from the source record into the destination record.
// Returns the destination cursor after the last step (unchanged if the range is empty).
static C_0082d410* __cdecl CopyStringElementsBackward(std::uint8_t* rangeStart, std::uint8_t* sourceEnd, C_0082d410* destEnd, int /*unusedArg4*/)
{
    constexpr std::size_t kRecordSize = 0x3C;
    constexpr std::size_t kTailOffset = 0x20;

    std::uint8_t* source = sourceEnd;
    C_0082d410* dest = destEnd;

    if (rangeStart != sourceEnd)
    {
        std::uint8_t* destBytes = reinterpret_cast<std::uint8_t*>(destEnd) + kTailOffset;
        std::uint8_t* srcBytes = sourceEnd + kTailOffset;

        do
        {
            source -= kRecordSize;
            dest = reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(dest) - kRecordSize);
            srcBytes -= kRecordSize;
            destBytes -= kRecordSize;

            AssignStringSubstring(dest, nullptr, source, 0u, 0xFFFFFFFFu);

            std::uint8_t* srcBase = srcBytes - 4;
            std::uint8_t* destBase = destBytes - 4;
            for (int i = 0; i < 8; ++i)
            {
                CSF_AT(std::uint32_t, destBase, i * 4) = CSF_AT(std::uint32_t, srcBase, i * 4);
            }
        } while (source != rangeStart);
    }

    return dest;
}

HOOK(0x006E19B0, CopyStringElementsBackward, "auto");
