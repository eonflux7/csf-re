#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833a48_Cleanup, 0x00491F50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_00833a48_Destroy(C_00833a48* self, void* /*edx*/, char flags)
{
    C_00833a48_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x00491F20, C_00833a48_Destroy, "auto");
