#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using NavigationDispatchTable = const std::uint32_t[4];
CSF_GLOBAL(NavigationDispatchTable, g_SliderNavigationDispatchTable, 0x0072E550);

CSF_FUNC(int, __fastcall, CGuiControl_IsNavigationTargetEligible, 0x00718700,
         (std::uint8_t* self, void* edx));

static std::uint8_t* __fastcall CGuiSlider_NavigateFromSelectedPart(
    CGuiSlider* self, void* /*edx*/, std::uint8_t* direction)
{
    auto* result = reinterpret_cast<std::uint8_t*>(self->m_pSelectedPart);
    if (!result)
        return result;

    result = direction;
    std::uint32_t slot;
    switch (reinterpret_cast<std::uintptr_t>(direction))
    {
    case 0: slot = 0x90 / 4; break;
    case 1: slot = 0x94 / 4; break;
    case 2: slot = 0x88 / 4; break;
    case 3: slot = 0x8C / 4; break;
    default: return result;
    }

    auto* selectedPart = reinterpret_cast<void*>(self->m_pSelectedPart);
    result = reinterpret_cast<std::uint8_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, selectedPart, 0)[slot])(selectedPart, nullptr);

    if (!result || !*result)
        return result;

    result = reinterpret_cast<std::uint8_t* (__fastcall*)(
        CGuiSlider*, void*, std::uint8_t*)>(
        CSF_AT(void**, self, 0)[0xFC / 4])(self, nullptr, result);

    auto* target = result;
    if (!target)
        return result;

    const int eligibility = CGuiControl_IsNavigationTargetEligible(target, nullptr);
    result = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(eligibility));

    // The original tests AL but retains the complete EAX return value.
    if (static_cast<std::uint8_t>(eligibility) == 0)
        return result;

    result = reinterpret_cast<std::uint8_t* (__fastcall*)(
        CGuiSlider*, void*, std::uint8_t*)>(
        CSF_AT(void**, self, 0)[0x158 / 4])(self, nullptr, target);

    auto* context = reinterpret_cast<void*>(self->field_0010);
    auto* contextData = CSF_AT(void*, context, 0);
    auto* receiver = CSF_AT(void*, contextData, 4);
    if (receiver)
    {
        result = reinterpret_cast<std::uint8_t* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, receiver, 0)[0x24 / 4])(receiver, nullptr, 0);
    }

    return result;
}

HOOK(0x0072E4A0, CGuiSlider_NavigateFromSelectedPart, "auto");
