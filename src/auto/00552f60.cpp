#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the heap-allocated name buffer (when capacity >= 16) and resets the
// string to its empty, inline-storage state: capacity 15, length 0, first byte 0.
static int __fastcall ResetSpawnNameBuffer(void* self, void* /*edx*/) {
    if (CSF_AT(uint32_t, self, 0x1b8) >= 0x10u) {
        uint32_t heapBuffer = CSF_AT(uint32_t, self, 0x1a4);
        if (heapBuffer != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(heapBuffer));
        }
    }
    CSF_AT(uint32_t, self, 0x1b8) = 0xF;
    CSF_AT(uint32_t, self, 0x1b4) = 0;
    CSF_AT(uint8_t, self, 0x1a4) = 0;

    SceneLoader_NoOp();
    return 0;
}

HOOK(0x00552F60, ResetSpawnNameBuffer, "auto");
