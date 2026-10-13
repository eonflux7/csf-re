#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies a 44-byte string record: assigns the substring [0, end) of the source
// string into the destination string, then copies the four trailing 32-bit fields.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static constexpr std::size_t kStringRecordSize = 44;

// Copies records from [rangeEnd - 44 .. rangeBegin) backward into destEnd - 44, returns the last written destination record.
static void* __cdecl RtFs_CopyStringRecordsBackward(void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
{
    if (rangeBegin == rangeEnd)
        return destEnd;

    std::uint8_t* src = static_cast<std::uint8_t*>(rangeEnd);
    std::uint8_t* dst = static_cast<std::uint8_t*>(destEnd);
    do
    {
        src -= kStringRecordSize;
        dst -= kStringRecordSize;
        AssignStringSubstring(dst, nullptr, src, 0, 0xFFFFFFFFu);
        CSF_AT(std::uint32_t, dst, 0x1C) = CSF_AT(std::uint32_t, src, 0x1C);
        CSF_AT(std::uint32_t, dst, 0x20) = CSF_AT(std::uint32_t, src, 0x20);
        CSF_AT(std::uint32_t, dst, 0x24) = CSF_AT(std::uint32_t, src, 0x24);
        CSF_AT(std::uint32_t, dst, 0x28) = CSF_AT(std::uint32_t, src, 0x28);
    } while (src != rangeBegin);

    return dst;
}

HOOK(0x0069EC50, RtFs_CopyStringRecordsBackward, "auto");
