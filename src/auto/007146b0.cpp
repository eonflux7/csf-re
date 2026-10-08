#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (CGuiControl* self, void* edx));

// Preserve the complete EAX value forwarded by the original target.
CSF_FUNC(int, __fastcall, CGuiControl_ProcessSelectionInput_718360, 0x00718360,
         (CGuiControl* self, void* edx, int input));

static int __fastcall CGuiListBoxBase_ProcessInput(
    CGuiListBoxBase* self, void* /*edx*/, int input)
{
    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, slider, 0)[0x18 / 4])(
                slider, nullptr, input);

        auto* origin = ((std::uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x148 / 4])(self, nullptr);

        std::uint32_t scratch[2];
        auto* extent =
            ((std::uint32_t*(__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, scratch);

        const std::uint32_t bottom = origin[1] + extent[1];
        const std::uint32_t right = origin[0] + extent[0];
        std::uint32_t corner[2];
        corner[1] = bottom;
        corner[0] = right;

        if (static_cast<std::uint8_t>(
                CGuiSlider_IsSelectableAndNotType10(self, nullptr)) != 0)
        {
            slider = reinterpret_cast<void*>(self->m_pSlider);
            if (((std::uint8_t(__fastcall*)(
                     void*, void*, std::uint32_t*))
                    CSF_AT(void**, slider, 0)[0x154 / 4])(
                        slider, nullptr, corner) != 0)
            {
                if (((std::uint8_t(__fastcall*)(void*, void*))
                        CSF_AT(void**, self, 0)[0xBC / 4])(
                            self, nullptr) == 0)
                {
                    CGuiSlider_ForwardSelfToParent(self, nullptr);
                }
            }
        }
    }

    return CGuiControl_ProcessSelectionInput_718360(self, nullptr, input);
}

HOOK(0x007146B0, CGuiListBoxBase_ProcessInput, "auto");
