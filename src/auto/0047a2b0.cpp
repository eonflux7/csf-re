#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008318c8_Cleanup, 0x00479BB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_008318c8_Destroy(C_008318c8* self, void* /*edx*/, char flags)
{
    C_008318c8_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x0047A2B0, C_008318c8_Destroy, "auto");
