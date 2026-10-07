#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall CGuiListBoxBase_SetSelected(
    CGuiListBoxBase* self, void* /*edx*/, int32_t selected)
{
    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider)
    {
        ((void(__fastcall*)(void*, void*, int32_t))
            CSF_AT(void**, slider, 0)[0xB8 / 4])(slider, nullptr, selected);
    }

    uint8_t result = self->m_bSelectable;
    if (result != 0 || static_cast<uint8_t>(selected) == 0)
    {
        self->m_bSelected = static_cast<uint8_t>(selected);
    }
    else
    {
        result = self->m_bSelected;
        // Preserve the original write-back even though the value is unchanged.
        *static_cast<volatile uint8_t*>(&self->m_bSelected) = result;
    }

    return result;
}

HOOK(0x00714380, CGuiListBoxBase_SetSelected, "auto");
