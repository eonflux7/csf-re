#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static int __fastcall ResolveHandleSlotIfValid(int* self, void* /*edx*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
    int slotIndex = CSF_AT(int, self, 0x0C);
    if (slotIndex >= 0) {
        // Resolve the handle stored at self+4 against the current scene's handle table.
        return CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)((uint8_t*)self + 4));
    }
    return slotIndex;
}

HOOK(0x0045DC80, ResolveHandleSlotIfValid, "auto");
