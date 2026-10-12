#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Element size of the copied record (0x34 bytes).
static constexpr std::uint32_t kStringRecordSize = 0x34;

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies records from the end of [firstRecord, endRecord) backward into destination, returning the
// last destination record written. When the range is empty, returns the destination unchanged.
static int __cdecl CopyStringArrayBackward(int firstRecord, int endRecord, int destinationEnd, int /*unused*/)
{
    std::uint32_t sourceCursor = static_cast<std::uint32_t>(endRecord);
    if (static_cast<std::uint32_t>(firstRecord) == sourceCursor)
        return destinationEnd;

    std::uint32_t destinationCursor = static_cast<std::uint32_t>(destinationEnd);
    do
    {
        sourceCursor -= kStringRecordSize;
        destinationCursor -= kStringRecordSize;

        std::uint8_t* sourceRecord = reinterpret_cast<std::uint8_t*>(sourceCursor);
        std::uint8_t* destRecord = reinterpret_cast<std::uint8_t*>(destinationCursor);

        AssignStringSubstring(destRecord, nullptr, sourceRecord, 0, static_cast<std::uint32_t>(-1));

        CSF_AT(std::uint32_t, destRecord, 0x1C) = CSF_AT(std::uint32_t, sourceRecord, 0x1C);
        CSF_AT(std::uint32_t, destRecord, 0x20) = CSF_AT(std::uint32_t, sourceRecord, 0x20);
        CSF_AT(std::uint32_t, destRecord, 0x24) = CSF_AT(std::uint32_t, sourceRecord, 0x24);
        CSF_AT(std::uint32_t, destRecord, 0x28) = CSF_AT(std::uint32_t, sourceRecord, 0x28);
        CSF_AT(std::uint32_t, destRecord, 0x2C) = CSF_AT(std::uint32_t, sourceRecord, 0x2C);
        CSF_AT(std::uint8_t, destRecord, 0x30) = CSF_AT(std::uint8_t, sourceRecord, 0x30);
        CSF_AT(std::uint8_t, destRecord, 0x31) = CSF_AT(std::uint8_t, sourceRecord, 0x31);
        CSF_AT(std::uint8_t, destRecord, 0x32) = CSF_AT(std::uint8_t, sourceRecord, 0x32);
    } while (sourceCursor != static_cast<std::uint32_t>(firstRecord));

    return static_cast<int>(destinationCursor);
}

HOOK(0x775440, CopyStringArrayBackward, "auto");
