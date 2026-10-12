#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ParticleEmitters_TeardownBuffer, 0x006E21C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8BB738, 0x008BB738);

// Releases the shared particle emitter buffer if one exists: runs its teardown
// routine, frees the block through the game heap, and clears the global pointer.
static int __cdecl ReleaseSharedEmitterBuffer()
{
    if (g_Value_8BB738 == 0)
        return 0;

    void* emitterBuffer = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB738));
    ParticleEmitters_TeardownBuffer(emitterBuffer, nullptr);
    int freeResult = Mem_Free(emitterBuffer);
    g_Value_8BB738 = 0;
    return freeResult;
}

HOOK(0x6E27A0, ReleaseSharedEmitterBuffer, "auto");
