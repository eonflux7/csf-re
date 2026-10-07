#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_UpdateSliderFromVisibleRowCount(
    CGuiListBoxBase* self, void* /*edx*/)
{
    if (self->m_pSlider == 0)
        return 0;

    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    std::uint32_t availableHeight = CSF_AT(std::uint32_t, dimensions, 4);
    const std::uint32_t adjustment178 = self->field_0178;
    const std::uint32_t adjustment174 = self->field_0174;
    const std::uint32_t rowHeight = self->field_01b8;
    availableHeight -= adjustment178;
    availableHeight -= adjustment174;

    const std::uint32_t rowStride = rowHeight + 2u * self->m_lineSpacing;
    const std::int32_t remainder =
        static_cast<std::int32_t>(availableHeight) %
        static_cast<std::int32_t>(rowStride);

    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    const std::uint32_t visibleRows =
        (availableHeight - static_cast<std::uint32_t>(remainder)) / rowStride;

    // Preserve the slider's vtable across the intervening list-box call.
    void** sliderVtable = CSF_AT(void**, slider, 0);
    const int sliderValue =
        reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t, int)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(
                self, nullptr, visibleRows, 0);

    slider = reinterpret_cast<void*>(self->m_pSlider);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        sliderVtable[0x260 / 4])(slider, nullptr, sliderValue);
}

HOOK(0x00714310, CGuiListBoxBase_UpdateSliderFromVisibleRowCount, "auto");
