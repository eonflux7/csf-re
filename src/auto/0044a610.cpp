#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083114c_Cleanup, 0x0044A490, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_0083114c_Destroy(C_0083114c* self, void* /*edx*/, char flags)
{
    C_0083114c_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x0044A610, C_0083114c_Destroy, "auto");
