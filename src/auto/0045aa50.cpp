#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns the scene handle stored in the record at self+4 when the signed
// field at self+0xC is non-negative; otherwise returns 0.
static int __fastcall HudStatusIcons_GetCurrentSceneIndex(int* self, void* /*edx*/)
{
    int result = 0;
    if (CSF_AT(int, self, 0xC) >= 0)
        return CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(self + 1));
    return result;
}

HOOK(0x0045AA50, HudStatusIcons_GetCurrentSceneIndex, "auto");
