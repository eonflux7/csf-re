#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// BOOL __thiscall sub_718700(_BYTE *this)  -- secondary gate check
CSF_FUNC(int, __fastcall, CGuiRadioGroup_CheckSecondaryState, 0x718700, (CGuiRadioGroup* self, void* edx));

// int __thiscall sub_72B2E0(int this, int a2, int a3)  -- actual event dispatch
CSF_FUNC(int, __fastcall, CGuiRadioGroup_DispatchToBase, 0x72B2E0, (CGuiRadioGroup* self, void* edx, int a2, int a3));

static int __fastcall CGuiRadioGroup_HandleEvent(CGuiRadioGroup* self, void* /*edx*/, int a2, int a3)
{
    void** vtbl = CSF_AT(void**, self, 0);

    int result = ((int(__fastcall*)(void*, void*))vtbl[0x6c / 4])(self, nullptr);
    if ((uint8_t)result)
    {
        result = CGuiRadioGroup_CheckSecondaryState(self, nullptr);
        if ((uint8_t)result)
            return CGuiRadioGroup_DispatchToBase(self, nullptr, a2, a3);
    }
    return result;
}

HOOK(0x72E870, CGuiRadioGroup_HandleEvent, "auto");
