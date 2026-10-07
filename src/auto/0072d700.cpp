#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00719530: char __thiscall CGuiControl_Update(CGuiControl* this, float dt)
CSF_FUNC(char, __fastcall, CGuiControl_Update, 0x00719530,
         (CGuiControl* self, void* edx, float dt));

// Forwards the frame/update time to the slider's three child elements
// (up arrow, down arrow, thumb) through vtable slot +0x13C, then updates the
// base control. Children are updated in field order, each one re-read from
// the slider after the previous call.
static char __fastcall CGuiSlider_UpdatePartsAndBase(
    CGuiSlider* self, void* /*edx*/, float dt)
{
    void* part = reinterpret_cast<void*>(self->m_pArrowUp);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, part, 0)[0x13C / 4])(part, nullptr, dt);
    }

    part = reinterpret_cast<void*>(self->m_pArrowDown);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, part, 0)[0x13C / 4])(part, nullptr, dt);
    }

    part = reinterpret_cast<void*>(self->m_pThumb);
    if (part) {
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, part, 0)[0x13C / 4])(part, nullptr, dt);
    }

    return CGuiControl_Update(self, nullptr, dt);
}

HOOK(0x0072D700, CGuiSlider_UpdatePartsAndBase, "auto");
