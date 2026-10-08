#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10,
         0x00718700, (CGuiControl* self, void* edx));

// Capture the entire EAX register: the target forwards it unchanged,
// even though this callee's inferred return type is char.
CSF_FUNC(int, __fastcall, CGuiControl_HandleMouseMovement,
         0x00719400, (CGuiControl* self, void* edx, int inputValue));

static int __fastcall CGuiListBoxBase_ForwardInputToSliderAndBase(
    CGuiListBoxBase* self, void* /*edx*/, int inputValue)
{
    int result =
        reinterpret_cast<int (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = CGuiSlider_IsSelectableAndNotType10(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result =
        reinterpret_cast<int (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, slider, 0)[0x34 / 4])(
                slider, nullptr, inputValue);
    }

    return CGuiControl_HandleMouseMovement(self, nullptr, inputValue);
}

HOOK(0x007141A0, CGuiListBoxBase_ForwardInputToSliderAndBase, "auto");
