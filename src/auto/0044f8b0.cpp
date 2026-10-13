#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the pointer stored at offset 0x1A4 (dword index 105) of the scheduler
// base object. Only a plain 32-bit load: the value is returned as-is, no checks.
static void* __fastcall SceneAccess_GetSceneState(void* self, void* /*edx*/) {
    return reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x1A4));
}

HOOK(0x0044F8B0, SceneAccess_GetSceneState, "auto");
