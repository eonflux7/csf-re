#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008310d8_Cleanup, 0x004451E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_008310d8_Destroy(C_008310d8* self, void* /*edx*/, char flags)
{
    C_008310d8_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x00445260, C_008310d8_Destroy, "auto");
