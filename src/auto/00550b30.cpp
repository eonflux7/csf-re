#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns 1 when the mode byte at +6 is 1, 2 or 3 (team modes), otherwise 0.
// __thiscall, ret 0: the object pointer arrives in ecx, nothing is popped.
static int __fastcall MpSpawnPoints_IsTeamMode(void* self, void* /*edx*/) {
    uint8_t modeByte = CSF_AT(uint8_t, self, 0x06);
    if (modeByte == 1 || modeByte == 2 || modeByte == 3) {
        return 1;
    }
    return 0;
}

HOOK(0x550B30, MpSpawnPoints_IsTeamMode, "auto");
