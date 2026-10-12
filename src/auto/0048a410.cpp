#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00833708_Cleanup, 0x0048A1E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static unsigned int* __fastcall C_00833708_Destroy(
    C_00833708* self, void* /*edx*/, char flags)
{
    C_00833708_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<unsigned int*>(self);
}

HOOK(0x0048A410, C_00833708_Destroy, "auto");
