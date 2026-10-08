#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiButton_ProcessPermanentQuad, 0x007260F0,
         (CGuiButton* self, void* edx));
CSF_FUNC(char, __fastcall, IsQuadHandledByEffect, 0x00731ED0,
         (void* self, void* edx, int* quad));
CSF_FUNC(void, __fastcall, CGuiButton_ProcessQuadWithScaledAlpha, 0x00724F90,
         (CGuiButton* self, void* edx, int* quad, float alpha));

static void __fastcall CGuiButton_ProcessCurrentStateQuad(
    CGuiButton* self, void* /*edx*/)
{
    CGuiButton_ProcessPermanentQuad(self, nullptr);

    int* hoverQuad = reinterpret_cast<int*>(self->m_pHoverQuad);
    void* effect = reinterpret_cast<void*>(self->m_pFx);
    if (IsQuadHandledByEffect(effect, nullptr, hoverQuad))
        return;

    const auto useOverride =
        reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4]);
    if (useOverride(self, nullptr)) {
        const auto getOverrideQuad =
            reinterpret_cast<int*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x264 / 4]);
        int* quad = getOverrideQuad(self, nullptr);
        CGuiButton_ProcessQuadWithScaledAlpha(self, nullptr, quad, 1.0f);
        return;
    }

    int* quad = nullptr;
    const std::uint32_t state = self->field_0148;
    if (state == 0) {
        quad = reinterpret_cast<int*>(self->m_pIdleQuad);
    } else if (state == 1 || state == 2) {
        if (state == 1)
            quad = reinterpret_cast<int*>(self->m_pHoverQuad);
        else
            quad = reinterpret_cast<int*>(self->m_pPressedQuad);

        if (!quad)
            quad = reinterpret_cast<int*>(self->m_pIdleQuad);
    }

    CGuiButton_ProcessQuadWithScaledAlpha(self, nullptr, quad, 1.0f);
}

HOOK(0x00726180, CGuiButton_ProcessCurrentStateQuad, "auto");
