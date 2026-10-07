#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall
CGuiListBoxBase_TailDispatchSliderOverflow(
    CGuiListBoxBase* /*self*/, void* /*edx*/)
{
    __asm {
        mov ecx, dword ptr [ecx + 0x134]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x134]
    }
}

static int __fastcall CGuiListBoxBase_HandleSliderOverflow(
    CGuiListBoxBase* self, void* /*edx*/)
{
    if (self->m_pSlider == 0)
        return 0;

    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    std::uint32_t availableHeightBits =
        CSF_AT(std::uint32_t, dimensions, 4);
    const std::uint32_t lowerInset = self->field_0178;
    const std::uint32_t upperInset = self->field_0174;
    const std::uint32_t rowHeight = self->field_01b8;

    availableHeightBits -= lowerInset;
    availableHeightBits -= upperInset;

    const std::uint32_t lineSpacing = self->m_lineSpacing;
    const std::int32_t rowStride =
        static_cast<std::int32_t>(rowHeight + lineSpacing * 2u);
    const std::int32_t availableHeight =
        static_cast<std::int32_t>(availableHeightBits);

    const std::int32_t remainder = availableHeight % rowStride;
    const std::int32_t completeHeight = static_cast<std::int32_t>(
        availableHeightBits - static_cast<std::uint32_t>(remainder));
    const std::int32_t visibleRows = completeHeight / rowStride;

    const int itemCount =
        reinterpret_cast<int (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

    if (itemCount <= visibleRows)
        return itemCount;

    // Require a tail transfer so the naked thunk retains the caller's stack.
    [[clang::musttail]]
    return CGuiListBoxBase_TailDispatchSliderOverflow(self, nullptr);
}

HOOK(0x00714640, CGuiListBoxBase_HandleSliderOverflow, "auto");
