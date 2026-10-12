#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00862d48_Cleanup, 0x006FC040, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_00862d48* __fastcall C_00862d48_Destroy(
    C_00862d48* self, void* /*edx*/, char flags)
{
    C_00862d48_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x006FC100, C_00862d48_Destroy, "auto");
