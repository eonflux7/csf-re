#include <cstdint>
#include "globals.h"

#include "hook.h"

// Returns the 16-bit loadout threshold stored for the given slot index.
// Thiscall with one stack argument (ret 4). Unknown indices return 0.
static int16_t __fastcall MpSpawnPoints_GetLoadoutThreshold(void* self, void* /*edx*/, int index) {
    switch (index) {
    case 0:
        return CSF_AT(int16_t, self, 0x68C);
    case 1:
        return CSF_AT(int16_t, self, 0x680);
    case 2:
        return CSF_AT(int16_t, self, 0x698);
    case 3:
        return CSF_AT(int16_t, self, 0x692);
    case 4:
        return CSF_AT(int16_t, self, 0x69E);
    case 5:
        return CSF_AT(int16_t, self, 0x686);
    default:
        return 0;
    }
}

HOOK(0x740CB0, MpSpawnPoints_GetLoadoutThreshold, "auto");
