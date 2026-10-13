#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, S_00693740_ClearState, 0x00693BD0, (void* self, void* edx));

static int __fastcall S_00693740_ReleaseBuffers(S_00693740* self, void* /*edx*/)
{
    S_00693740_ClearState(self, nullptr);

    if (self->field_002c)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }

    int result = 0;
    if (self->field_001c)
    {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }
    return result;
}

HOOK(0x00694380, S_00693740_ReleaseBuffers, "auto");
