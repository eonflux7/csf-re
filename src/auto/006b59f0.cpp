#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

static int __fastcall S_006b59f0_ClearRecordStorage(
    S_006b59f0* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        GfxTextureLoader_ResetRecordRange(
            reinterpret_cast<int*>(result),
            reinterpret_cast<int*>(self->field_0008));

        result = static_cast<int>(self->field_0004);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return result;
}

HOOK(0x006B59F0, S_006b59f0_ClearRecordStorage, "auto");
