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

// Callees (bound by address, signatures copied from the packet).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(char*, __fastcall, NetPerfCounters_CopyRecordHeader, 0x0078BDA0, (void* self, void* edx, int a2));

// Copies a range of 0xA4-byte records backwards from rangeEnd into destEnd.
// Each record: first dword copied directly, a sub-object at +4 copied via
// NetPerfCounters_CopyRecordHeader, and a string at +0x88 copied via AssignStringSubstring.
static std::uint32_t* __cdecl NetPerfCounters_CopyRecordRangeReverse(std::uint32_t* rangeBegin, std::uint32_t* rangeEnd, std::uint32_t* destEnd, int /*unused*/)
{
    std::uint8_t* begin = reinterpret_cast<std::uint8_t*>(rangeBegin);
    std::uint8_t* srcCur = reinterpret_cast<std::uint8_t*>(rangeEnd);

    if (begin == srcCur)
        return destEnd;

    std::uint8_t* dstCur = reinterpret_cast<std::uint8_t*>(destEnd);
    do
    {
        std::uint32_t firstDword = *reinterpret_cast<std::uint32_t*>(srcCur - 0xA4);
        srcCur -= 0xA4;
        dstCur -= 0xA4;
        *reinterpret_cast<std::uint32_t*>(dstCur) = firstDword;

        NetPerfCounters_CopyRecordHeader(dstCur + 4, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(srcCur + 4)));
        AssignStringSubstring(dstCur + 0x88, nullptr, srcCur + 0x88, 0u, 0xFFFFFFFFu);
    } while (srcCur != begin);

    return reinterpret_cast<std::uint32_t*>(dstCur);
}

HOOK(0x78C240, NetPerfCounters_CopyRecordRangeReverse, "auto");
