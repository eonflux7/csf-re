#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083f2f0_Cleanup, 0x004FB4B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_0083f2f0_Destroy(C_0083f2f0* self, void* /*edx*/, char flags)
{
    C_0083f2f0_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x004FB6E0, C_0083f2f0_Destroy, "auto");
