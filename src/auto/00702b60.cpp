#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __stdcall GfxParticleAtomic_ReleaseBlock(int block) {
    int result = block;
    if (block != 0) {
        return CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(block)));
    }
    return result;
}

HOOK(0x00702B60, GfxParticleAtomic_ReleaseBlock, "auto");
