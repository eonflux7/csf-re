#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00862c74_Cleanup, 0x006FB4C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_00862c74_Destroy(C_00862c74* self, void* /*edx*/, char flags)
{
    C_00862c74_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x006FB490, C_00862c74_Destroy, "auto");
