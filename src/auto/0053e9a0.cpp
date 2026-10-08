#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the object held at this+0x84 (ecx = object, void return, no stack args).
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ReleaseOwnedObject, 0x00436E20, (uint32_t* self, void* edx));

// Refreshes the panel for the given step index (ecx = this, one stack arg, ret 4).
// The original also passes edi/esi implicitly; see notes.
CSF_FUNC(void, __fastcall, CMenuMissionInfo_RefreshPanelAtIndex, 0x0053E2F0, (CMenuMissionInfo* self, void* edx, int index));

// If the object at this+0x84 reports ready (vtable slot 0xAC), consumes one pending
// step at this+0x88 and refreshes the panel with the decremented count. Otherwise
// releases the object at this+0x84.
static void __fastcall CMenuMissionInfo_ProcessPendingPageStep(CMenuMissionInfo* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0084);
    void** ownedVtable = CSF_AT(void**, ownedObject, 0);
    auto isOwnedObjectReady = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(ownedVtable[0xAC / 4]);

    if (isOwnedObjectReady(ownedObject, nullptr) != 0)
    {
        uint32_t pendingSteps = self->field_0088;
        if (pendingSteps != 0)
        {
            uint32_t remainingSteps = pendingSteps - 1;
            self->field_0088 = remainingSteps;
            CMenuMissionInfo_RefreshPanelAtIndex(self, nullptr, static_cast<int>(remainingSteps));
        }
    }
    else
    {
        CMenuMissionInfo_ReleaseOwnedObject(reinterpret_cast<uint32_t*>(self->field_0084), nullptr);
    }
}

HOOK(0x0053E9A0, CMenuMissionInfo_ProcessPendingPageStep, "auto");
