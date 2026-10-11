#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, AttackTasks_ReleaseSubobject, 0x0047F7B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Steps back 4 bytes to the block header, runs the sub-object release,
// and frees the block when the low bit of the flag argument is set.
static char* __fastcall AttackTasks_DestroyWithOptionalFree(char* self, void* /*edx*/, char freeFlag)
{
    char* base = self - 4;
    AttackTasks_ReleaseSubobject(base, nullptr);
    if ((freeFlag & 1) != 0)
        Mem_Free(base);
    return base;
}

HOOK(0x0047F770, AttackTasks_DestroyWithOptionalFree, "auto");
