#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_UpdateAndRenderDebugRect, 0x00719810, (CGuiControl* self, void*));

// The pointer is opaque; matching signatures permit a guaranteed tail transfer.
static __declspec(naked) int __fastcall CGuiControl_TailRenderEffects(
    CGuiControl* /*effects*/, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x30]
    }
}

static int __fastcall CGuiControl_PrepareListRendering(
    CGuiControl* self, void* /*edx*/)
{
    CGuiControl_UpdateAndRenderDebugRect(self, nullptr);

    const std::uint32_t effects = self->m_pEffects;
    if (!effects)
        return 0;

    [[clang::musttail]] return CGuiControl_TailRenderEffects(
        reinterpret_cast<CGuiControl*>(effects), nullptr);
}

HOOK(0x00719A20, CGuiControl_PrepareListRendering, "auto");
