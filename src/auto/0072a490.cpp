#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiPanel_SetSelected(
    CGuiPanel* self, void* /*edx*/, std::uint32_t selected)
{
    void* control = reinterpret_cast<void*>(self->field_014c);
    if (!control)
        return;

    void* target =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[0x104 / 4])(control, nullptr);

    if (!target)
        target = reinterpret_cast<void*>(self->field_014c);

    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, target, 0)[0xB8 / 4])(target, nullptr, selected);

    std::uint8_t selectedByte = static_cast<std::uint8_t>(selected);
    if (!self->m_bSelectable && selectedByte)
        selectedByte = self->m_bSelected;

    self->m_bSelected = selectedByte;
}

HOOK(0x0072A490, CGuiPanel_SetSelected, "auto");
