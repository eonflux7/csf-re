#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __stdcall ReleaseVectorStorage(void* block, int /*unused*/) {
    if (block == nullptr) {
        return block;
    }
    return reinterpret_cast<void*>(static_cast<uintptr_t>(Mem_FreeDebug(block)));
}

HOOK(0x006C8D30, ReleaseVectorStorage, "auto");
