#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833708_GetEntryValue(C_00833708* self, void* /*edx*/, int index)
{
    const uint32_t offset = static_cast<uint32_t>(index) * 96;
    return *reinterpret_cast<int*>(self->field_000c + offset);
}

HOOK(0x00489D50, C_00833708_GetEntryValue, "auto");
