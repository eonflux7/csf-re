#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl GfxTextureLoader_ResetRecordRange(int* begin, int* end)
{
    int result = 0;
    for (int* record = begin; record != end; record += 4)
    {
        result = record[1];
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(record[1]));

        record[1] = 0;
        record[2] = 0;
        record[3] = 0;
    }
    return result;
}

HOOK(0x006DEC40, GfxTextureLoader_ResetRecordRange, "auto");
