#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CGuiControl_FindEntryById, 0x00452A30, (CGuiControl* self, void* , unsigned int entryId));

static bool __stdcall Gui_IsHandleEntryShared(int handle)
{
    void* control = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!control || (CSF_AT(std::uint32_t, control, 0x170) & 0x2000) == 0)
        return false;

    const unsigned int entryId = CSF_AT(unsigned int, control, 0x1F4);
    if (entryId == static_cast<unsigned int>(-1))
        return false;

    void* entry = reinterpret_cast<void*>(CGuiControl_FindEntryById(
        static_cast<CGuiControl*>(g_pCurrentScene), nullptr, entryId));
    return entry && CSF_AT(std::uint32_t, entry, 0xC) > 1u;
}

HOOK(0x004483F0, Gui_IsHandleEntryShared, "auto");
