#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (int* begin, int* end, void* container, CGuiListBoxBase* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x00718C00,
         (CGuiControl* self, void* edx, int event));

static int __fastcall CGuiListBoxBase_ClearItems(
    CGuiListBoxBase* self, void* /*edx*/)
{
    CGuiControl_ResetInput(self, nullptr);

    self->m_bLayoutDirty = 1;
    self->field_01dc = 1;

    auto* itemContainer = reinterpret_cast<std::uint8_t*>(self) + 0x188;
    if (self->field_018c != 0) {
        ResetRecordRange(
            reinterpret_cast<int*>(self->field_018c),
            reinterpret_cast<int*>(self->field_0190),
            itemContainer, self);

        if (self->field_018c != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_018c));
    }

    self->field_018c = 0;
    self->field_0190 = 0;
    CSF_AT(std::uint32_t, self, 0x194) = 0;

    if (self->field_01ac != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_01ac));

    self->field_01ac = 0;
    self->field_01b0 = 0;
    self->field_01b4 = 0;

    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider) {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, slider, 0)[0x16C / 4])(slider, nullptr);
    }

    slider = reinterpret_cast<void*>(self->m_pSlider);
    std::uint32_t sliderValue;
    if (slider) {
        sliderValue =
            reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, slider, 0)[0x270 / 4])(slider, nullptr);
    } else {
        sliderValue = self->field_0168;
    }

    self->field_0168 = sliderValue;
    self->field_01f0 = 1.0f;
    return CGuiControl_ForwardEventToFirstEffect(self, nullptr, 0x3F800000);
}

HOOK(0x00716320, CGuiListBoxBase_ClearItems, "auto");
