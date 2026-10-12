#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083f0ec_Cleanup, 0x004F2F10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_0083f0ec_Destroy(C_0083f0ec* self, void* /*edx*/, char flags)
{
    C_0083f0ec_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x004F3260, C_0083f0ec_Destroy, "auto");
