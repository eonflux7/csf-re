#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Byte flag at 0x008BAEB0 (g_unk_8BAD98 + 280). Case 4 sets it, case 5 clears it.
CSF_GLOBAL(uint8_t, g_MemDebugAllocationFlag, 0x008BAEB0);

static int MemDebug_SetAllocationMode(int mode) {
    int result = mode;
    switch (mode) {
    case 4:
        g_MemDebugAllocationFlag = 1;
        break;
    case 5:
        g_MemDebugAllocationFlag = 0;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x006910B0, MemDebug_SetAllocationMode, "auto");
