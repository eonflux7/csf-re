#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static void __cdecl HudStatusIcons_UpdateOpacity(
    void* icon, float deltaTime, int /*unused*/)
{
    if (!icon)
        return;

    const int32_t fadeDurationBits = CSF_AT(int32_t, icon, 0x18);
    CSF_AT(float, icon, 0x04) = 255.0f;

    if (fadeDurationBits != -1082130432)
    {
        const float elapsed = deltaTime + CSF_AT(float, icon, 0x0C);
        CSF_AT(float, icon, 0x0C) = elapsed;

        const float fadeDuration = CSF_AT(float, icon, 0x18);
        if (!(elapsed >= fadeDuration))
        {
            CSF_AT(float, icon, 0x04) =
                elapsed / fadeDuration * g_ColorChannelScale;
        }
        else
        {
            CSF_AT(float, icon, 0x0C) = 0.0f;
            CSF_AT(float, icon, 0x18) = -1.0f;
        }
        return;
    }

    if (CSF_AT(int32_t, icon, 0x08) == -1082130432)
        return;

    const float period = CSF_AT(float, icon, 0x08);
    float elapsed = deltaTime + CSF_AT(float, icon, 0x0C);
    CSF_AT(float, icon, 0x0C) = elapsed;

    if (elapsed >= period)
        elapsed = g_fZero;

    CSF_AT(float, icon, 0x0C) = elapsed;

    const float halfPeriod = period * g_HalfDurationScale;
    if (halfPeriod > elapsed)
    {
        CSF_AT(float, icon, 0x04) =
            (halfPeriod - elapsed) / halfPeriod * g_ColorChannelScale;
    }
    else
    {
        CSF_AT(float, icon, 0x04) =
            (elapsed - halfPeriod) / halfPeriod * g_ColorChannelScale;
    }
}

HOOK(0x00458210, HudStatusIcons_UpdateOpacity, "auto");
