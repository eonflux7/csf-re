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

// Element stride of the graph node records copied here: 0x48 bytes = 18 dwords.
static constexpr int kNodeRecordDwords = 18;

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, WaypointGraph_CopyEdgeContainer, 0x0075C120, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, WaypointGraph_CopyLinkContainer, 0x0075DAA0, (void* self, void* edx, void* a4a));

// Copies node records from [rangeBegin, rangeEnd) into the buffer ending at destEnd,
// walking backwards one record at a time. Returns the new beginning of the copied range
// (or destEnd unchanged when the source range is empty).
static std::uint32_t* __cdecl WaypointGraph_CopyNodeRangeBackward(std::uint32_t* rangeBegin, std::uint32_t* rangeEnd, std::uint32_t* destEnd, int /*unusedArg*/)
{
    if (rangeBegin == rangeEnd) {
        return destEnd;
    }

    std::uint32_t* src = rangeEnd;
    std::uint32_t* dst = destEnd;
    do {
        src -= kNodeRecordDwords;
        dst -= kNodeRecordDwords;

        // +0x00: node id
        dst[0] = src[0];

        // +0x04: name string
        AssignStringSubstring(reinterpret_cast<C_0082d410*>(dst + 1), nullptr, src + 1, 0, 0xFFFFFFFFu);

        // +0x20: scalar field
        dst[8] = src[8];

        // +0x24: edge container
        WaypointGraph_CopyEdgeContainer(dst + 9, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(src + 9)));

        // +0x34: link container
        WaypointGraph_CopyLinkContainer(dst + 13, nullptr, src + 13);

        // +0x44: scalar field
        dst[17] = src[17];
    } while (src != rangeBegin);

    return dst;
}

HOOK(0x0075DF20, WaypointGraph_CopyNodeRangeBackward, "auto");
