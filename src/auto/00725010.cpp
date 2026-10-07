#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Clone of CGuiButton_ProcessQuadWithScaledAlpha (0x724F90) that uses a
// different pair of quad vtable slots (0x64 get-color / 0x60 set-color).
// Reads the quad's current colour, scales its alpha by
// (opacity * self->field_01d0), applies the adjusted colour, invokes the
// self virtual method at vtable offset 0x238 passing the quad, then restores
// the original colour.
static void __fastcall CGuiButton_ProcessQuadWithScaledAlpha2(
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
            CSF_AT(void**, quad, 0)[0x64 / 4])(quad, nullptr);

    const double alphaScale =
        static_cast<double>(opacity) * static_cast<double>(self->field_01d0);

    Color originalColor = *currentColor;
    Color adjustedColor = originalColor;
    adjustedColor.alpha =
        static_cast<float>(alphaScale * static_cast<double>(originalColor.alpha));

    ((void (__fastcall*)(void*, void*, Color*))
        CSF_AT(void**, quad, 0)[0x60 / 4])(quad, nullptr, &adjustedColor);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, quad);

    ((void (__fastcall*)(void*, void*, Color*))
        CSF_AT(void**, quad, 0)[0x60 / 4])(quad, nullptr, &originalColor);
}

HOOK(0x00725010, CGuiButton_ProcessQuadWithScaledAlpha2, "auto");
