#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x0053D740, (CMenuMissionInfo* self, void* , int));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ResetPanels, 0x0053DAA0, (CMenuMissionInfo* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static void __fastcall CMenuMissionInfo_HandleEvent(
    CMenuMissionInfo* self, void* /*edx*/, char active)
{
    self->field_00a4 = active;

    bool destinationAvailable = false;
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x110)
        && self->field_00a4
        && static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()))
    {
        void* processor = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        if (UmsVirtualProcessor_GetDefaultDestination(processor, nullptr))
        {
            processor = reinterpret_cast<void*>(GetGlobalValue_8B0568());
            void* destination =
                UmsVirtualProcessor_GetDefaultDestination(processor, nullptr);
            if (CSF_AT(uint32_t, destination, 0x1C) == 0)
                destinationAvailable = true;
        }
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateObject = CSF_AT(void*, globalState, 0x18);
    CSF_AT(uint8_t, stateObject, 0x271) = destinationAvailable;

    if (self->field_00a5)
    {
        self->field_00a5 = 0;
        self->field_00a6 = 1;
    }
    else
    {
        CMenuMissionInfo_EnterPanelState(self, nullptr, 0);
        void* panel = reinterpret_cast<void*>(self->field_001c);
        self->field_0014 = 0;
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, 1);

        if (self->field_00a4)
            self->vProcessGlobalState();
        else
            CMenuMissionInfo_ResetPanels(self, nullptr);
    }
}

HOOK(0x0053DC60, CMenuMissionInfo_HandleEvent, "auto");
