#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// BOOL __thiscall sub_718700(_BYTE *this)  -- secondary gate check
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x718700, (CGuiRadioGroup* self, void* edx));

// char __thiscall sub_72B660(int this, int a2, int a3)  -- actual event dispatch to base
CSF_FUNC(int, __fastcall, CGuiPanel_HandleDirectionalInput, 0x72B660, (CGuiRadioGroup* self, void* edx, int a2, int a3));

static int __fastcall CGuiRadioGroup_HandleEvent(CGuiRadioGroup* self, void* /*edx*/, int a2, int a3)
{
    void** vtbl = CSF_AT(void**, self, 0);

    int result = ((int(__fastcall*)(void*, void*))vtbl[0x6c / 4])(self, nullptr);
    if ((uint8_t)result)
    {
        result = CGuiSlider_IsSelectableAndNotType10(self, nullptr);
        if ((uint8_t)result)
            return CGuiPanel_HandleDirectionalInput(self, nullptr, a2, a3);
    }
    return result;
}

HOOK(0x72E8A0, CGuiRadioGroup_HandleEvent, "auto");
