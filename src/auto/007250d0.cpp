#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Identical signature permits a guaranteed tail transfer.
// The first argument is an opaque quad pointer, not a button.
static __declspec(naked) int __fastcall CGuiButton_ForwardQuadQuery(
    CGuiButton* /*quad*/, void* /*edx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        mov eax, 0
        jmp dword ptr [edx + 0x7c]
    }
}

static int __fastcall CGuiButton_QueryStateQuad(
    CGuiButton* self, void* /*edx*/)
{
    const int guardResult =
        reinterpret_cast<int (__fastcall*)(CGuiButton*, void*)>(
            CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(guardResult) != 0)
        return guardResult;

    std::uint32_t stateResult = CSF_AT(uint32_t, self, 0x148);
    void* quad;

    if (stateResult == 0)
    {
        quad = reinterpret_cast<void*>(self->m_pIdleQuad);
    }
    else
    {
        --stateResult;
        if (stateResult == 0)
        {
            quad = reinterpret_cast<void*>(self->m_pHoverQuad);
        }
        else
        {
            --stateResult;
            if (stateResult != 0)
                return static_cast<int>(stateResult);

            quad = reinterpret_cast<void*>(self->m_pPressedQuad);
        }

        if (quad == nullptr)
            quad = reinterpret_cast<void*>(self->m_pIdleQuad);
    }

    if (quad == nullptr)
        return static_cast<int>(stateResult);

    [[clang::musttail]] return CGuiButton_ForwardQuadQuery(
        reinterpret_cast<CGuiButton*>(quad), nullptr);
}

HOOK(0x007250D0, CGuiButton_QueryStateQuad, "auto");
