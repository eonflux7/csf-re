#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x00774C10
// Returns the signed number of 44-byte entries between the begin pointer at
// +0x04 and the end pointer at +0x08, or 0 when the begin pointer is null.
static int __fastcall WaypointGraph_GetSize(S_0051aa70* self, void* /*edx*/)
{
    const int32_t begin = static_cast<int32_t>(self->field_0004);
    if (begin == 0)
        return 0;

    const int32_t end = static_cast<int32_t>(self->field_0008);
    return (end - begin) / 44;
}

HOOK(0x00774C10, WaypointGraph_GetSize, "auto");
