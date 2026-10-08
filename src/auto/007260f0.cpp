#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ProcessQuadWithScaledAlpha, 0x00724F90,
         (CGuiButton* self, void* edx, void* quad, float alpha));
CSF_FUNC(int, __fastcall, PreparePermanentQuad, 0x00725B40,
         (CGuiButton* self, void* edx));
CSF_FUNC(char, __fastcall, IsQuadEffectActive, 0x00731E60,
         (void* self, void* edx, void* quad));

CSF_GLOBAL(float, g_fInv255, 0x00830A00);

static void __fastcall CGuiButton_ProcessPermanentQuad(
    CGuiButton* self, void* /*edx*/)
{
    void* quad = reinterpret_cast<void*>(self->m_pPermanentQuad);
    if (!quad)
        return;

    if (self->m_pText != 0)
    {
        const float alpha = static_cast<float>(
            static_cast<double>(self->field_01d8) * g_fInv255);
        ProcessQuadWithScaledAlpha(self, nullptr, quad, alpha);
        return;
    }

    if (IsQuadEffectActive(
            reinterpret_cast<void*>(self->m_pFx), nullptr, quad) != 0)
        return;

    const unsigned char useFullAlpha =
        reinterpret_cast<unsigned char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);

    if (useFullAlpha != 0)
    {
        PreparePermanentQuad(self, nullptr);
        ProcessQuadWithScaledAlpha(
            self, nullptr,
            reinterpret_cast<void*>(self->m_pPermanentQuad), 1.0f);
        return;
    }

    const float height = self->field_01d8;
    quad = reinterpret_cast<void*>(self->m_pPermanentQuad);
    const float alpha =
        static_cast<float>(static_cast<double>(height) * g_fInv255);
    ProcessQuadWithScaledAlpha(self, nullptr, quad, alpha);
}

HOOK(0x007260F0, CGuiButton_ProcessPermanentQuad, "auto");
