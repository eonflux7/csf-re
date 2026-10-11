#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00831120_Cleanup, 0x00446740, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_00831120_Destroy(C_00831120* self, void* /*edx*/, char flags)
{
    C_00831120_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x00446D00, C_00831120_Destroy, "auto");
