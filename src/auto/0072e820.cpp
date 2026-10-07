#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetSelectionState, 0x00725120,
         (CGuiControl* self, void* edx, int state));
CSF_FUNC(int, __fastcall, CGuiControl_AssignSelectedControl, 0x0072A4F0,
         (CGuiControl* self, void* edx, CGuiControl* control));

static int __fastcall CGuiRadioGroup_SelectControl(
    CGuiRadioGroup* self, void* /*edx*/, CGuiControl* control)
{
    if (((CGuiControl* (__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x104 / 4])(self, nullptr))
    {
        CGuiControl* previousControl =
            ((CGuiControl* (__fastcall*)(void*, void*))
                 CSF_AT(void**, self, 0)[0x104 / 4])(self, nullptr);
        CGuiControl_SetSelectionState(previousControl, nullptr, 0);
    }

    int result = CGuiControl_AssignSelectedControl(self, nullptr, control);
    if (control)
        return CGuiControl_SetSelectionState(control, nullptr, 1);
    return result;
}

HOOK(0x0072E820, CGuiRadioGroup_SelectControl, "auto");
