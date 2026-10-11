#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall int GuiControl_GetChildByIndex(this, unsigned index), ret 4.
// Returns the handle-table entry for the child at `index`, or 0 when index is -1 or out of range.
static int __fastcall GuiControl_GetChildByIndex(void* self, void* /*edx*/, uint32_t index)
{
    int result = 0;
    if (index != 0xFFFFFFFFu && index < CSF_AT(uint32_t, self, 0x0C))
    {
        uint32_t entriesBase = CSF_AT(uint32_t, self, 0x04);
        uint32_t entryAddress = entriesBase + 12u * index;
        return CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(entryAddress));
    }
    return result;
}

HOOK(0x004A0F00, GuiControl_GetChildByIndex, "auto");
