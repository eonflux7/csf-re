#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void, __stdcall, ApplyControlAssociation, 0x00448450, (int a2, void* arg4));
CSF_FUNC(int, __fastcall, FindAssociatedControl, 0x00449390, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FindAssociationGroup, 0x0044A580, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static void __fastcall S_00448830_UpdateControlAssociation(
    S_00448830* self, void* /*edx*/, int handle)
{
    void* control = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!control || (CSF_AT(uint32_t, control, 0x170) & 0x2000) == 0)
        return;

    int groupId = CSF_AT(int, control, 0x1F4);
    if (groupId == -1)
        return;

    void* group = reinterpret_cast<void*>(
        FindAssociationGroup(self, nullptr, groupId));
    void* associatedControl = reinterpret_cast<void*>(
        FindAssociatedControl(group, nullptr, handle));
    ApplyControlAssociation(handle, associatedControl);
}

HOOK(0x0044A640, S_00448830_UpdateControlAssociation, "auto");
