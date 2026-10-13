#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x0044BB60
// Returns the signed element count of the range [field_0004, field_0008),
// divided by 100 (signed division, truncating toward zero), or 0 when
// field_0004 is null.
static int __fastcall GfxCollisionShapes_GetSize(S_0044bb60* self, void* /*edx*/)
{
    const int32_t begin = static_cast<int32_t>(self->field_0004);
    if (begin == 0)
        return 0;

    const int32_t end = static_cast<int32_t>(self->field_0008);
    return (end - begin) / 100;
}

HOOK(0x0044BB60, GfxCollisionShapes_GetSize, "auto");
