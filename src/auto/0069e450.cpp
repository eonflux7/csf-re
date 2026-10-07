#include <cstdint>
#include "globals.h"
#include "hook.h"

// Import pointer for the Windows Sleep API (kernel32), resolved by the loader.
// typedef avoids the comma inside the macro argument list.
typedef void (__stdcall* SleepFn)(uint32_t);
CSF_GLOBAL(SleepFn, g_pfnSleep, 0x82D05C);

// Wrapper around Sleep: the original loads the IAT slot and tail-calls through it,
// using __cdecl (ret with no immediate, caller cleans the stack).
static void __cdecl GameSleepMilliseconds(uint32_t dwMilliseconds)
{
    g_pfnSleep(dwMilliseconds);
}

HOOK(0x69E450, GameSleepMilliseconds, "auto");
