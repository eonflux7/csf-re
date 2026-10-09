#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, StringAllocatorReset, 0x005112A0, (void* self, void* edx));

// Resets a string-like object to empty. Heap-allocated storage (capacity >= 16)
// is released with Mem_FreeDebug; the buffer is then pointed back at the inline
// storage, the capacity is set to 15, and the size to 0.
static int __stdcall WeaponDefs_ResetStringToEmpty(void* self) {
    if (CSF_AT(uint32_t, self, 0x20c) >= 0x10u && CSF_AT(uint32_t, self, 0x1f8) != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x1f8)));
    }
    CSF_AT(uint32_t, self, 0x20c) = 15;
    CSF_AT(uint32_t, self, 0x208) = 0;
    CSF_AT(uint8_t, self, 0x1f8) = 0;
    return StringAllocatorReset(static_cast<uint8_t*>(self) + 4, nullptr);
}

HOOK(0x00783620, WeaponDefs_ResetStringToEmpty, "auto");
