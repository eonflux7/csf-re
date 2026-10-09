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

// Import slot for kernel32!SetThreadPriority (IAT entry at 0x82D208).
CSF_GLOBAL(void*, g_pfnSetThreadPriority, 0x82D208);

using SetThreadPriorityFn = int(__stdcall*)(void* hThread, int nPriority);

static char __fastcall CSceneLoader_SetLoadThreadIdlePriority(CSceneLoader* self, void* /*edx*/)
{
    // Flag byte at +0xB5: when clear, the function returns 0 without touching the thread.
    uint8_t priorityEnabled = CSF_AT(uint8_t, self, 0xB5);
    char result = 0;

    if (priorityEnabled != 0)
    {
        // Thread handle stored at +0x110.
        uint32_t threadHandle = CSF_AT(uint32_t, self, 0x110);
        if (threadHandle != 0)
        {
            // THREAD_PRIORITY_IDLE (-15); the low byte of the BOOL result is returned.
            SetThreadPriorityFn setThreadPriority = (SetThreadPriorityFn)g_pfnSetThreadPriority;
            result = static_cast<char>(
                setThreadPriority(reinterpret_cast<void*>(static_cast<uintptr_t>(threadHandle)), -15));
        }
    }

    return result;
}

HOOK(0x00505CB0, CSceneLoader_SetLoadThreadIdlePriority, "auto");
