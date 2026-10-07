#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiSlider_SetThumb(
    CGuiSlider* self, void* /*edx*/, CGuiControl* thumb)
{
    auto* previousThumb =
        reinterpret_cast<CGuiControl*>(self->m_pThumb);

    if (previousThumb) {
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousThumb, 0)[0])(
                previousThumb, nullptr, 1);
        self->m_pThumb = 0;
    }

    self->m_pThumb = reinterpret_cast<std::uint32_t>(thumb);
    if (!thumb)
        return;

    ((void(__fastcall*)(void*, void*, CGuiSlider*))
        CSF_AT(void**, thumb, 0)[0x108 / 4])(
            thumb, nullptr, self);

    auto* currentThumb =
        reinterpret_cast<CGuiControl*>(self->m_pThumb);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, currentThumb, 0)[0xB0 / 4])(
            currentThumb, nullptr, 1);

    currentThumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
    std::uint32_t origin[2] = {0, 0};
    ((int(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, currentThumb, 0)[0x98 / 4])(
            currentThumb, nullptr, origin);
}

HOOK(0x0072D990, CGuiSlider_SetThumb, "auto");
