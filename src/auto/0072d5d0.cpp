#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0072D5D0 : CGuiSlider vtable slot. Selects (or deselects) the currently
// active part of the slider (arrow/thumb) and mirrors the resulting flag into
// the slider's own m_bSelected byte.
//
//   part = self->m_pSelectedPart;              // +0x154
//   if (!part) return;
//   target = part->vtbl[0x104/4](part, a2);    // "get part for context"
//   if (!target) target = self->m_pSelectedPart;
//   target->vtbl[0xB8/4](target, selected);    // "set selected" on the part
//   v = selected;
//   if (!self->m_bSelectable && selected)      // +0x115, +0xd4
//       v = self->m_bSelected;
//   self->m_bSelected = v;
//
// NOTE: the original is a __userpurge with an extra register argument (a2)
// arriving in ebx; the natural C++ mapping keeps it in the second __fastcall
// register slot (edx) and forwards it unchanged to the 0x104 virtual call.
static void __fastcall CGuiSlider_SetSelected(
    CGuiSlider* self, void* hitContext, std::uint8_t selected)
{
    void* part = reinterpret_cast<void*>(self->m_pSelectedPart);
    if (!part)
        return;

    void* target = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, part, 0)[0x104 / 4])(part, hitContext);

    if (!target)
        target = reinterpret_cast<void*>(self->m_pSelectedPart);

    ((void (__fastcall*)(void*, void*, std::uint8_t))
        CSF_AT(void**, target, 0)[0xB8 / 4])(target, nullptr, selected);

    std::uint8_t value = selected;
    if (!self->m_bSelectable && value)
        value = self->m_bSelected;

    self->m_bSelected = value;
}

HOOK(0x0072D5D0, CGuiSlider_SetSelected, "auto");
