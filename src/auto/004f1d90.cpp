#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall HudStringShow_GetRangeEntryCount(void* self, void* /*edx*/)
{
    uint32_t begin = CSF_AT(uint32_t, self, 0x4);
    if (begin == 0)
        return 0;

    uint32_t end = CSF_AT(uint32_t, self, 0x8);
    int byteSpan = static_cast<int32_t>(end - begin);
    return byteSpan / 124;
}

HOOK(0x4F1D90, HudStringShow_GetRangeEntryCount, "auto");
