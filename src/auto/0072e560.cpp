#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));

using SliderNavigationJumpTable = std::uint32_t[9];
CSF_GLOBAL(SliderNavigationJumpTable, g_SliderNavigationJumpTable, 0x0072E614);

static std::uint8_t* __fastcall CGuiSlider_NavigateFromSelectedPart(
    CGuiSlider* self, void* /*edx*/, int eventId)
{
    if (self->m_pSelectedPart == 0)
        return nullptr;

    const std::uint32_t dispatchIndex =
        static_cast<std::uint32_t>(eventId) - 200u;

    std::uint32_t navigationSlot;
    switch (dispatchIndex)
    {
    case 0:
        navigationSlot = 0x90 / 4;
        break;
    case 3:
        navigationSlot = 0x88 / 4;
        break;
    case 5:
        navigationSlot = 0x8C / 4;
        break;
    case 8:
        navigationSlot = 0x94 / 4;
        break;
    default:
        return reinterpret_cast<std::uint8_t*>(dispatchIndex);
    }

    auto* selectedPart =
        reinterpret_cast<CGuiControl*>(self->m_pSelectedPart);

    auto* result =
        ((std::uint8_t*(__fastcall*)(CGuiControl*, void*))
             CSF_AT(void**, selectedPart, 0)[navigationSlot])(
            selectedPart, nullptr);

    if (result == nullptr || *result == 0)
        return result;

    auto* target =
        ((CGuiControl*(__fastcall*)(CGuiSlider*, void*, std::uint8_t*))
             CSF_AT(void**, self, 0)[0xFC / 4])(
            self, nullptr, result);

    if (target == nullptr)
        return nullptr;

    const int navigationAllowed = CGuiSlider_IsSelectableAndNotType10(target, nullptr);
    result = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uint32_t>(navigationAllowed));

    if (static_cast<std::uint8_t>(navigationAllowed) == 0)
        return result;

    result =
        ((std::uint8_t*(__fastcall*)(CGuiSlider*, void*, CGuiControl*))
             CSF_AT(void**, self, 0)[0x158 / 4])(
            self, nullptr, target);

    void* context = reinterpret_cast<void*>(self->field_0010);
    void* contextHeader = CSF_AT(void*, context, 0);
    void* navigationManager = CSF_AT(void*, contextHeader, 4);

    if (navigationManager != nullptr)
    {
        result =
            ((std::uint8_t*(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, navigationManager, 0)[0x24 / 4])(
                navigationManager, nullptr, 0);
    }

    return result;
}

HOOK(0x0072E560, CGuiSlider_NavigateFromSelectedPart, "auto");
