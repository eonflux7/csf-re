#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __stdcall GfxClumpDict_ResetEntryAndFreeBuffers(int* a1) {
    int result = CSF_AT(int, a1, 0x20);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, a1, 0x20))));
        CSF_AT(int, a1, 0x20) = 0;
    }
    if (static_cast<unsigned int>(CSF_AT(int, a1, 0x18)) >= 0x10) {
        result = CSF_AT(int, a1, 0x04);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, a1, 0x04))));
        }
    }
    CSF_AT(int, a1, 0x18) = 0x0F;
    CSF_AT(int, a1, 0x14) = 0;
    CSF_AT(uint8_t, a1, 0x04) = 0;
    return result;
}

HOOK(0x006D49D0, GfxClumpDict_ResetEntryAndFreeBuffers, "auto");
