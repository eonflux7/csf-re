#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, CGuiControl_ContainsPoint, 0x007185A0,
         (CGuiControl* self, void* edx, std::uint32_t* point));

static int __fastcall CGuiSlider_ContainsPoint(
    CGuiSlider* self, void* /*edx*/, std::uint32_t* point)
{
    using ContainsPointMethod =
        std::uint8_t (__fastcall*)(void*, void*, std::uint32_t*);

    bool containsPoint = false;

    auto* arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
    if (arrowUp) {
        containsPoint = reinterpret_cast<ContainsPointMethod>(
            CSF_AT(void**, arrowUp, 0)[0x154 / 4])(
                arrowUp, nullptr, point) != 0;
    }

    auto* arrowDown = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
    if (arrowDown) {
        if (containsPoint ||
            reinterpret_cast<ContainsPointMethod>(
                CSF_AT(void**, arrowDown, 0)[0x154 / 4])(
                    arrowDown, nullptr, point) != 0) {
            containsPoint = true;
        }
    }

    auto* thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
    if (thumb) {
        if (containsPoint ||
            reinterpret_cast<ContainsPointMethod>(
                CSF_AT(void**, thumb, 0)[0x154 / 4])(
                    thumb, nullptr, point) != 0) {
            return 1;
        }
    } else if (containsPoint) {
        return 1;
    }

    return CGuiControl_ContainsPoint(self, nullptr, point) ? 1 : 0;
}

HOOK(0x0072D680, CGuiSlider_ContainsPoint, "auto");
