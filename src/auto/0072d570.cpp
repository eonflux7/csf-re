#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetPosition_7198A0, 0x007198A0,
         (CGuiControl* self, void* edx, float first, float second));

static int __fastcall CGuiSlider_ApplyFloatPair(
    CGuiSlider* self, void* /*edx*/, float first, float second)
{
    CGuiControl_SetPosition_7198A0(
        reinterpret_cast<CGuiControl*>(self), nullptr, first, second);

    void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
    if (arrowUp != nullptr)
    {
        ((int(__fastcall*)(void*, void*, float, float))
            CSF_AT(void**, arrowUp, 0)[0x150 / 4])(
                arrowUp, nullptr, first, second);
    }

    void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrowDown != nullptr)
    {
        ((int(__fastcall*)(void*, void*, float, float))
            CSF_AT(void**, arrowDown, 0)[0x150 / 4])(
                arrowDown, nullptr, first, second);
    }

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb != nullptr)
    {
        return ((int(__fastcall*)(void*, void*, float, float))
            CSF_AT(void**, thumb, 0)[0x150 / 4])(
                thumb, nullptr, first, second);
    }

    return 0;
}

HOOK(0x0072D570, CGuiSlider_ApplyFloatPair, "auto");
