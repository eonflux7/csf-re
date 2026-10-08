#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto CGuiControl_ApplySetting =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*, int)>(CSF_ADDR_FN(0x00718C00));

static int __fastcall CGuiSlider_ApplySettingToParts(
    CGuiSlider* self, void* /*edx*/, int setting)
{
    void* part = reinterpret_cast<void*>(self->m_pArrowUp);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, part, 0)[0x1AC / 4])(part, nullptr, setting);
    }

    part = reinterpret_cast<void*>(self->m_pArrowDown);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, part, 0)[0x1AC / 4])(part, nullptr, setting);
    }

    part = reinterpret_cast<void*>(self->m_pThumb);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, part, 0)[0x1AC / 4])(part, nullptr, setting);
    }

    return CGuiControl_ApplySetting(self, nullptr, setting);
}

HOOK(0x0072D870, CGuiSlider_ApplySettingToParts, "auto");
