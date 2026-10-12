#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_004482c0_GetEntryCount(S_004482c0* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0004;
    if (begin == 0)
        return 0;

    return static_cast<int32_t>(self->field_0008 - begin) / 20;
}

HOOK(0x004482C0, S_004482c0_GetEntryCount, "auto");
