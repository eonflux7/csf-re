#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CGuiSlider_ForwardThumbListRendering(
    CGuiSlider* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x134]
    }
}

static int __fastcall CGuiSlider_RefreshStandardState(
    CGuiSlider* self, void* /*edx*/)
{
    auto* arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
    if (arrowUp)
        arrowUp->vPrepareListRendering();

    auto* arrowDown = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
    if (arrowDown)
        arrowDown->vPrepareListRendering();

    const uint32_t thumb = self->m_pThumb;
    if (thumb) {
        [[clang::musttail]] return CGuiSlider_ForwardThumbListRendering(
            reinterpret_cast<CGuiSlider*>(thumb), nullptr);
    }

    return 0;
}

HOOK(0x0072CF80, CGuiSlider_RefreshStandardState, "auto");
