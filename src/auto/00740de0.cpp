#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// __thiscall(ecx = this), 2 stack args, ret 8.
// If the pending-save flag at +0x6F0 is set, clear it, copy the 16-bit value at +0x446
// into *a2 (when a2 is non-null) and the dword at +0x6EC into *a3 (when a3 is non-null).
// Returns the low byte of a3 when the flag was set, otherwise 0.
static char __fastcall MpSpawnPoints_SavePlayerState(void* self, void* /*edx*/,
                                                     uint32_t* outPlayerSlot,
                                                     uint32_t* outSavedValue)
{
    char result = CSF_AT(char, self, 0x6F0);
    if (result)
    {
        CSF_AT(uint8_t, self, 0x6F0) = 0;
        if (outPlayerSlot)
            *outPlayerSlot = static_cast<uint32_t>(CSF_AT(uint16_t, self, 0x446));
        result = static_cast<char>(reinterpret_cast<uintptr_t>(outSavedValue));
        if (outSavedValue)
            *outSavedValue = CSF_AT(uint32_t, self, 0x6EC);
    }
    return result;
}

HOOK(0x740DE0, MpSpawnPoints_SavePlayerState, "auto");
