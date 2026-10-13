#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);

// Advances the highlight fade of an objective entry. While the elapsed fade time
// is below the fade duration, pushes a white RGBA color whose alpha ramps down
// across the duration to the color target (vtable slot 26), then adds timeStep to
// the elapsed time. Once the fade has finished, resets the elapsed time and sets
// the state field to 2.
static void __fastcall HudObjectives_AdvanceHighlightFade(S_004276c0* self, void* /*edx*/, float timeStep)
{
    if (self->field_0044 < self->field_003c)
    {
        void* colorTarget = reinterpret_cast<void*>(self->field_0020);

        float colorValues[4];
        float halfDuration = self->field_003c * g_HalfDurationScale;
        colorValues[0] = g_ColorWhite * g_ColorChannelScale;
        colorValues[1] = g_ColorWhite_G * g_ColorChannelScale;
        colorValues[2] = g_ColorWhite_B * g_ColorChannelScale;
        colorValues[3] = g_ColorChannelScale
            - (self->field_0044 - halfDuration) * g_ColorChannelScale / halfDuration;

        ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, colorTarget, 0)[26])(
            colorTarget, nullptr, colorValues);

        self->field_0044 = timeStep + self->field_0044;
    }
    else
    {
        self->field_0044 = 0.0f;
        self->field_0000 = 2;
    }
}

HOOK(0x004280A0, HudObjectives_AdvanceHighlightFade, "auto");
