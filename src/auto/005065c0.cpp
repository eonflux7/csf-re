#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original 0x00505E00: __thiscall-style worker (ecx = this, edx unused). Returns int in eax.
CSF_FUNC(int, __fastcall, SceneLoader_RunLoadWorker, 0x00505E00, (void* self, void* edx));

// Thread-entry wrapper: forwards the thread parameter to the load worker and returns its result.
static int __stdcall SceneLoader_LoadThreadEntry(CSceneLoader* lpThreadParameter)
{
    return SceneLoader_RunLoadWorker(lpThreadParameter, nullptr);
}

HOOK(0x005065C0, SceneLoader_LoadThreadEntry, "auto");
