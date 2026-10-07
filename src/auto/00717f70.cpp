#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CGuiControl_ForwardInputResetToEffects(
    CGuiControl*, void*)
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0x128]
        test ecx, ecx
        jne dispatch_effects
        ret
    dispatch_effects:
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x2c]
    }
}

static int __fastcall CGuiControl_ResetInput(CGuiControl* self, void* edx)
{
    self->m_bMouseEnabled = 1;
    self->m_inputState = 2;
    self->field_012c = 0;

    // Require a jump, not a call, so the dispatcher receives the caller's stack.
    [[clang::musttail]]
    return CGuiControl_ForwardInputResetToEffects(self, edx);
}

HOOK(0x717F70, CGuiControl_ResetInput, "auto");
