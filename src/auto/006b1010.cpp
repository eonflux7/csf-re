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

CSF_FUNC(unsigned int, __fastcall, S_006b0320_ClearState, 0x006B0B10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall S_006b0320_ReleaseBuffers(S_006b0320* self, void* /*edx*/)
{
    S_006b0320_ClearState(self, nullptr);

    if (self->field_0010)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    // The original keeps field_0000 in v2 and returns its low byte, which is
    // overwritten by Mem_Free's result when the block was non-null.
    char result = 0;
    if (self->field_0000)
    {
        result = static_cast<char>(Mem_Free(reinterpret_cast<void*>(self->field_0000)));
        self->field_0000 = 0;
    }
    return result;
}

HOOK(0x006B1010, S_006b0320_ReleaseBuffers, "auto");
