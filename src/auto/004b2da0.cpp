#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_DestroyOrderTask, 0x004B2CF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall TOrden_Destroy(TOrden* self, void* /*edx*/, char flags)
{
    AiCore_DestroyOrderTask(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x004B2DA0, TOrden_Destroy, "auto");
