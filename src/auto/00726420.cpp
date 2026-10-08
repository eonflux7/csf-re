#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00719A20 — CGuiControl-level helper reset (ret 0, __thiscall)
CSF_FUNC(int, __fastcall, CGuiListBoxBase_PrepareListRendering, 0x719A20, (CGuiButton* self, void* edx));
// 0x00726180 — CGuiButton quad-state reset helper (ret 0, __thiscall)
CSF_FUNC(void, __fastcall, CGuiButton_ProcessCurrentStateQuad, 0x726180, (CGuiButton* self, void* edx));
// 0x00726210 — CGuiButton text/color-state reset helper (ret 0, __thiscall)
CSF_FUNC(void, __fastcall, CGuiButton_UpdateQuad, 0x726210, (CGuiButton* self, void* edx));

// If the control is not in its "active" state (virtual slot +0xAC returns 0), re-apply the
// default state: common control reset followed by the button quad/text state resets.
static void __fastcall CGuiButton_ResetStatesWhenInactive(CGuiButton* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    uint8_t isActive = ((uint8_t(__fastcall*)(void*, void*))vtbl[0xac / 4])(self, nullptr);
    if (isActive == 0)
    {
        CGuiListBoxBase_PrepareListRendering(self, nullptr);
        CGuiButton_ProcessCurrentStateQuad(self, nullptr);
        CGuiButton_UpdateQuad(self, nullptr);
    }
}

HOOK(0x726420, CGuiButton_ResetStatesWhenInactive, "auto");
