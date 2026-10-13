#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, HudStringShow_ClearDebugOverlay, 0x004F2F90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_ReleaseSharedTable, 0x004F4800, (void* self, void* edx));

// Clears the debug overlays of the two HUD string objects at +4 and +8, then
// tail-jumps into the shared-table release routine with the object at +12.
static uint32_t* __fastcall SceneLoader_ResetSharedResources(void* self, void* /*edx*/)
{
    HudStringShow_ClearDebugOverlay(CSF_AT(void*, self, 0x4), nullptr);
    HudStringShow_ClearDebugOverlay(CSF_AT(void*, self, 0x8), nullptr);
    return SceneLoader_ReleaseSharedTable(CSF_AT(void*, self, 0xC), nullptr);
}

HOOK(0x004F4950, SceneLoader_ResetSharedResources, "auto");
