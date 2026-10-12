#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_441F90, 0x00441F90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8afc80, 0x008AFC80);

// Releases the scene-load buffer if one is allocated: runs its teardown routine,
// frees the block through the game heap, and clears the global pointer.
static int __cdecl DynamicObj_ReleaseSceneLoadBuffer()
{
    if (g_Value_8afc80 == 0)
        return 0;

    void* loadBuffer = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8afc80));
    sub_441F90(loadBuffer, nullptr);
    int freeResult = Mem_Free(loadBuffer);
    g_Value_8afc80 = 0;
    return freeResult;
}

HOOK(0x442790, DynamicObj_ReleaseSceneLoadBuffer, "auto");
