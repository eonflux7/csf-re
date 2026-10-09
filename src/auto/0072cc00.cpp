#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiSlider_ResetInput(CGuiSlider* self, void* /*edx*/)
{
    void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
    self->m_pos = 0;
    self->m_fractionalPosition = 0.0f;

    if (arrowUp)
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, arrowUp, 0)[0x16C / 4])(arrowUp, nullptr);

    void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrowDown)
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, arrowDown, 0)[0x16C / 4])(arrowDown, nullptr);

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb)
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, thumb, 0)[0x16C / 4])(thumb, nullptr);

    return CGuiControl_ResetInput(self, nullptr);
}

HOOK(0x0072CC00, CGuiSlider_ResetInput, "auto");
