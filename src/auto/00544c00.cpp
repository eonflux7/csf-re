#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// std::string-style assign(src, pos, count) on a 36-byte record; returns the record pointer (unused here).
CSF_FUNC(void*, __fastcall, StringAssignRange, 0x4109E0,
         (void* self, void* edx, void* src, uint32_t pos, uint32_t count));

// Copies the records in [first, last) backward so that the last source record lands just before dest_last.
// Returns the new front of the destination range, or dest_last when the source range is empty.
static void* __cdecl CopyRecordsBackward(void* first, void* last, void* dest_last)
{
    if (first == last)
        return dest_last;

    uint8_t* src = static_cast<uint8_t*>(last);
    uint8_t* dst = static_cast<uint8_t*>(dest_last);
    do {
        src -= 36;
        dst -= 36;
        StringAssignRange(dst, nullptr, src, 0u, 0xFFFFFFFFu);
        dst[28] = src[28];
        dst[29] = src[29];
        dst[30] = src[30];
        dst[31] = src[31];
        dst[32] = src[32];
    } while (src != static_cast<uint8_t*>(first));

    return dst;
}

HOOK(0x00544C00, CopyRecordsBackward, "auto");
