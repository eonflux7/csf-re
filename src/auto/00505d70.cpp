#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees one embedded MSVC-style string buffer (heap block when capacity >= 16) and resets it to empty.
// The inline buffer lives at bufOff; its first dword holds the heap pointer when capacity >= 16.
static uint32_t ReleaseAndResetStringSlot(uint8_t* self, uint32_t result,
                                          int capOff, int sizeOff, int bufOff, int flagOff) {
    if (CSF_AT(uint32_t, self, capOff) >= 0x10) {
        result = CSF_AT(uint32_t, self, bufOff);
        if (result) {
            result = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(result)));
        }
    }
    CSF_AT(uint32_t, self, capOff) = 15;
    CSF_AT(uint32_t, self, sizeOff) = 0;
    CSF_AT(uint8_t, self, flagOff) = 0;
    return result;
}

static uint32_t __fastcall SceneLoader_ResetStrings(uint8_t* self, void* /*edx*/) {
    uint32_t result = CSF_AT(uint32_t, self, 0x6C);
    result = ReleaseAndResetStringSlot(self, result, 0x6C, 0x68, 0x58, 0x58);
    result = ReleaseAndResetStringSlot(self, result, 0x50, 0x4C, 0x3C, 0x3C);
    result = ReleaseAndResetStringSlot(self, result, 0x34, 0x30, 0x20, 0x20);
    result = ReleaseAndResetStringSlot(self, result, 0x18, 0x14, 0x04, 0x04);
    return result;
}

HOOK(0x00505D70, SceneLoader_ResetStrings, "auto");
