#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall ReleaseGameDataRecordLinks(S_0051aa70* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        UpdateGameDataRecordLinks(result, static_cast<int>(self->field_0008),
                                  reinterpret_cast<int>(self), 0);
        result = static_cast<int>(self->field_0004);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return result;
}

HOOK(0x0051AA70, ReleaseGameDataRecordLinks, "auto");
