#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SetPanelValue, 0x00436C50, (void* self, void* edx, float a2));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x005231D0, (int handle, void* edx, int value));
CSF_FUNC(char, __fastcall, CMenuMissionInfo_UpdatePanels, 0x0053D910, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ClosePanelAndDeactivate, 0x0053DBF0, (CMenuMissionInfo* self, void*));

static char __fastcall CMenuMissionInfo_SetContextAndRefreshPanels(
    CMenuMissionInfo* self, void* /*edx*/, char* context)
{
    const std::uint32_t contextBits = reinterpret_cast<std::uint32_t>(context);
    self->field_0080 = contextBits;

    const int handle = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(handle, nullptr, static_cast<int>(contextBits));
    Nullsub_506AC0();

    // The original passes the argument's raw bits, not a numeric conversion.
    float panelValue;
    std::memcpy(&panelValue, &contextBits, sizeof(panelValue));
    SetPanelValue(reinterpret_cast<void*>(self->field_0084), nullptr, panelValue);

    CMenuMissionInfo_ClosePanelAndDeactivate(self, nullptr);
    return CMenuMissionInfo_UpdatePanels(self, nullptr);
}

HOOK(0x0053DDE0, CMenuMissionInfo_SetContextAndRefreshPanels, "auto");
