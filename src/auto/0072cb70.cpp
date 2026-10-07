#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_CGuiSliderVtable, 0x00865E38);

CSF_FUNC(int, __fastcall, CGuiControl_Dtor, 0x00719B10,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiSlider_Destroy(
    CGuiSlider* self, void* /*edx*/)
{
    void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
    CSF_AT(void**, self, 0) = &g_CGuiSliderVtable;

    if (arrowUp != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, arrowUp, 0)[0])(
                arrowUp, nullptr, 1);
        self->m_pArrowUp = 0;
    }

    void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrowDown != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, arrowDown, 0)[0])(
                arrowDown, nullptr, 1);
        self->m_pArrowDown = 0;
    }

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, thumb, 0)[0])(
                thumb, nullptr, 1);
        self->m_pThumb = 0;
    }

    return CGuiControl_Dtor(
        reinterpret_cast<CGuiControl*>(self), nullptr);
}

HOOK(0x0072CB70, CGuiSlider_Destroy, "auto");
