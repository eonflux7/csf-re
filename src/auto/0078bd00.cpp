#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));

// Resets a string-like member block to empty. Heap-allocated storage (capacity >= 16)
// is released with Mem_FreeDebug; the buffer is then pointed back at the inline
// storage, the capacity is set to 15, and the size to 0. The embedded options
// storage at +4 is then destroyed and its result returned.
static int __stdcall NetPerfCounters_ResetStringToEmpty(void* self) {
    if (CSF_AT(uint32_t, self, 0xa0) >= 0x10u && CSF_AT(uint32_t, self, 0x8c) != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8c)));
    }
    CSF_AT(uint32_t, self, 0xa0) = 15;
    CSF_AT(uint32_t, self, 0x9c) = 0;
    CSF_AT(uint8_t, self, 0x8c) = 0;
    return static_cast<int>(DestroySceneObjectOptionsStorage(static_cast<uint8_t*>(self) + 4, nullptr));
}

HOOK(0x0078BD00, NetPerfCounters_ResetStringToEmpty, "auto");
