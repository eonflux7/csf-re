#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Win32 SetThreadPriority, reached through the import slot at 0x0082D208.
CSF_GLOBAL(void*, g_SetThreadPriorityIat, 0x82D208);

// Raises the priority of the scene loader's worker thread when the flag at +0xB5 is set.
// Returns the flag value when it is clear, otherwise the SetThreadPriority result.
static char __fastcall CSceneLoader_BoostWorkerThreadPriorityIfFlagged(CSceneLoader* self, void* /*edx*/) {
    char flagValue = CSF_AT(char, self, 0xB5);
    if (flagValue) {
        void* workerThreadHandle = CSF_AT(void*, self, 0x110);
        using SetThreadPriorityFn = int(__stdcall*)(void*, int);
        auto setThreadPriority = reinterpret_cast<SetThreadPriorityFn>(g_SetThreadPriorityIat);
        return static_cast<char>(setThreadPriority(workerThreadHandle, 2));
    }
    return flagValue;
}

HOOK(0x505CD0, CSceneLoader_BoostWorkerThreadPriorityIfFlagged, "auto");
