#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ApplyLinkedValue, 0x00718C00,
         (CGuiControl* self, void* edx, int value));

static int __fastcall CGuiListBoxBase_SetSliderLinkedValue(
    CGuiListBoxBase* self, void* /*edx*/, int value)
{
    void* slider = reinterpret_cast<void*>(self->m_pSlider);

    // Preserve the argument's bits rather than converting it to float.
    std::memcpy(&self->field_01f0, &value, sizeof(value));

    if (slider)
    {
        auto setter = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, slider, 0)[0x1AC / 4]);
        setter(slider, nullptr, value);
    }

    return CGuiControl_ApplyLinkedValue(self, nullptr, value);
}

HOOK(0x007143D0, CGuiListBoxBase_SetSliderLinkedValue, "auto");
