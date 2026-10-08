#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_PrepareListRendering, 0x00719A20,
         (CGuiSlider* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_RefreshStandardState, 0x0072CF80,
         (CGuiSlider* self, void* edx));
CSF_FUNC(char, __fastcall, RefreshChildControlStates, 0x0072CFD0,
         (CGuiSlider* self, void* edx));

static char __fastcall CGuiSlider_RefreshState(CGuiSlider* self, void* /*edx*/)
{
    char result =
        reinterpret_cast<char(__fastcall*)(CGuiSlider*, void*)>(
            CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);

    if (result == 0)
    {
        CGuiListBoxBase_PrepareListRendering(self, nullptr);

        if (self->m_bConsole != 0)
            result = RefreshChildControlStates(self, nullptr);
        else
            result = static_cast<char>(
                CGuiSlider_RefreshStandardState(self, nullptr));

        self->field_016a = 0;
    }

    return result;
}

HOOK(0x0072D0C0, CGuiSlider_RefreshState, "auto");
