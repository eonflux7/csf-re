#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, Waypoint_CopyBlock36, 0x0075C120, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Waypoint_CopyBlock52, 0x0075DAA0, (void* self, void* edx, void* a4a));

static std::uint8_t* WaypointGraph_AsBytePtr(int addr)
{
    return reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(addr)));
}

// Copies 72-byte waypoint records from the source record into every record in [dstBegin, dstEnd).
static int __cdecl WaypointGraph_CopyEntryRange(int dstBegin, int dstEnd, int src)
{
    int result = dstEnd;
    for (int i = dstBegin; i != dstEnd; i += 72)
    {
        std::uint8_t* item = WaypointGraph_AsBytePtr(i);
        std::uint8_t* srcRec = WaypointGraph_AsBytePtr(src);

        CSF_AT(int, item, 0) = CSF_AT(int, srcRec, 0);
        AssignStringSubstring(item + 4, nullptr, srcRec + 4, 0, 0xFFFFFFFFu);
        CSF_AT(int, item, 0x20) = CSF_AT(int, srcRec, 0x20);
        Waypoint_CopyBlock36(item + 36, nullptr, src + 36);
        Waypoint_CopyBlock52(item + 52, nullptr, reinterpret_cast<void*>(srcRec + 52));
        result = dstEnd;
        CSF_AT(int, item, 0x44) = CSF_AT(int, srcRec, 0x44);
    }
    return result;
}

HOOK(0x75E0B0, WaypointGraph_CopyEntryRange, "auto");
