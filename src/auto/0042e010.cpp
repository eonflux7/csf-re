// Commandos: Strike Force — HUD module.
// Clears the pending widget state object stored at +0x2CC, then raises the two
// HUD visibility flags in the global block (via GetGlobal8afc78).
// The original tail-jumps into 0x427BC0, whose body is reproduced inline here.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x42D6D0 : returns the pointer to the global HUD state block.
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());

// Body of 0x427BC0, entered with the pending-state object in ecx.
static void Hud_ResetPendingStateObject(uint8_t* pendingState)
{
    CSF_AT(int32_t, pendingState, 0x00) = -1;
    CSF_AT(uint32_t, pendingState, 0x44) = 0;
    CSF_AT(uint8_t, pendingState, 0x09) = 0;

    uint8_t* globalsBlock = reinterpret_cast<uint8_t*>(GetGlobal8afc78());
    CSF_AT(uint8_t, globalsBlock, 0x2A2) = 1;

    globalsBlock = reinterpret_cast<uint8_t*>(GetGlobal8afc78());
    CSF_AT(uint8_t, globalsBlock, 0x2A1) = 1;
}

static void __fastcall Hud_ClearPendingWidgetState(void* self, void* /*edx*/)
{
    uint8_t* pendingState = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x2CC));
    if (pendingState)
        Hud_ResetPendingStateObject(pendingState);
}

HOOK(0x42E010, Hud_ClearPendingWidgetState, "auto");
