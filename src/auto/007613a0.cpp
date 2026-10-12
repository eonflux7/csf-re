#include <cstdint>
#include "globals.h"
#include "hook.h"

// Autonomous AI action-state name table: g_psz_DISTRAIDO is an array of
// char* indexed by state id (4-byte stride).
CSF_GLOBAL(char*, g_psz_DISTRAIDO, 0x008A961C);

static char* AiAutonomous_GetActionStateName(int stateIndex) {
    return (&g_psz_DISTRAIDO)[stateIndex];
}

HOOK(0x007613A0, AiAutonomous_GetActionStateName, "auto");
