#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiButton_ProcessQuadWithScaledAlpha(
    CGuiButton* self, void* /*edx*/, void* quad, float opacity)
{
    if (!quad)
        return;

    struct Color {
        std::uint32_t red;
        std::uint32_t green;
        std::uint32_t blue;
        float alpha;
    };

    const Color* currentColor =
        ((const Color* (__fastcall*)(void*, void*))
            CSF_AT(void**, quad, 0)[0x6C / 4])(quad, nullptr);

    const double alphaScale =
        static_cast<double>(opacity) * static_cast<double>(self->field_01d0);

    Color originalColor = *currentColor;
    Color adjustedColor = originalColor;
    adjustedColor.alpha =
        static_cast<float>(alphaScale * static_cast<double>(originalColor.alpha));

    ((void (__fastcall*)(void*, void*, Color*))
        CSF_AT(void**, quad, 0)[0x68 / 4])(quad, nullptr, &adjustedColor);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, quad);

    ((void (__fastcall*)(void*, void*, Color*))
        CSF_AT(void**, quad, 0)[0x68 / 4])(quad, nullptr, &originalColor);
}

HOOK(0x00724F90, CGuiButton_ProcessQuadWithScaledAlpha, "auto");
