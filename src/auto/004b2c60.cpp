#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_DestroyAlertTask, 0x004B2AB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TAlerta_Destroy(TAlerta* self, void* /*edx*/, char flags)
{
    AiCore_DestroyAlertTask(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<char*>(self);
}

HOOK(0x004B2C60, TAlerta_Destroy, "auto");
