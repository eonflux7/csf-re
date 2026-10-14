#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_ScaleXZ_0_45, 0x00833E00);
CSF_GLOBAL(double, g_LightModulationPeriod, 0x0083E7B0);
CSF_GLOBAL(float, g_FilmRandomScale, 0x00846234);
CSF_GLOBAL(float, g_FilmDriftScale, 0x00862BE8);
CSF_GLOBAL(float, g_FilmRestartDelay, 0x00862BF0);
CSF_GLOBAL(float, g_FilmPatchAlphaBase, 0x00862BF4);
CSF_GLOBAL(float, g_FilmPatchSizeBase, 0x00862BFC);
CSF_GLOBAL(float, g_FilmSpeckSizeBase, 0x00862C14);   // 0.005 (0x6FA953, 0x6FAA50)
CSF_GLOBAL(float, g_FilmSpeckAlphaBase, 0x00862C00);  // 0.2 (0x6FAA8A)
CSF_GLOBAL(float, g_FilmScratchAlphaBase, 0x00862C1C);
CSF_GLOBAL(float, g_FilmSpeckAlphaScale, 0x00862C34);
CSF_GLOBAL(float, g_FilmSpeckSizeScale, 0x00862C38);
CSF_GLOBAL(float, g_FilmQuadExtentScale, 0x00862C3C);
CSF_GLOBAL(float, g_FilmQuadSizeScale, 0x00862C40);
CSF_GLOBAL(float, g_FilmPatchAlphaScale, 0x00862C44);
CSF_GLOBAL(float, g_FilmPatchSizeScale, 0x00862C48);
CSF_GLOBAL(float, g_FilmRestartDelayScale, 0x00862C4C);
CSF_GLOBAL(float, g_FilmInactiveDelayScale, 0x00862C50);
CSF_GLOBAL(float, g_FilmScratchAlphaScale, 0x00862C54);

static int __fastcall C_00862c74_UpdateFilmNoise(
    C_00862c74* self, void* /*edx*/, float elapsedSeconds)
{
    const float elapsedMilliseconds = elapsedSeconds * g_flt_0082E25C;
    void* controller = reinterpret_cast<void*>(self->field_0080);
    reinterpret_cast<int(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, controller, 0)[0x10 / 4])(
            controller, nullptr, elapsedMilliseconds);

    int count = static_cast<int32_t>(self->field_0088);
    if (count != 0)
        count = static_cast<int32_t>(self->field_008c - self->field_0088) / 20;

    uint32_t offset = 0;
    while (count > 0)
    {
        auto* scratch = reinterpret_cast<uint8_t*>(self->field_0088) + offset;
        const float remaining = CSF_AT(float, scratch, 0x04) - elapsedMilliseconds;
        CSF_AT(float, scratch, 0x04) = remaining;

        if (remaining > g_fZero)
        {
            const float drift = elapsedMilliseconds * g_ScaleXZ_0_45;
            const int driftRandom = _rand();
            CSF_AT(float, scratch, 0x00) =
                (drift - static_cast<float>(driftRandom) * elapsedMilliseconds *
                             g_FilmRandomScale) *
                    CSF_AT(float, scratch, 0x0C) * g_FilmDriftScale +
                CSF_AT(float, scratch, 0x00);

            const int alphaRandom = _rand();
            float alpha = static_cast<float>(alphaRandom) *
                              g_FilmScratchAlphaScale * g_ColorChannelScale +
                          g_FilmScratchAlphaBase;
            if (alpha > g_ColorChannelScale)
                alpha = g_ColorChannelScale;
            CSF_AT(uint8_t, scratch, 0x10) =
                static_cast<uint8_t>(static_cast<int64_t>(alpha));
        }
        else if (CSF_AT(float, scratch, 0x08) < g_fZero)
        {
            const int delayRandom = _rand();
            CSF_AT(float, scratch, 0x08) =
                static_cast<float>(delayRandom) * g_FilmInactiveDelayScale;
        }
        else if (remaining < -CSF_AT(float, scratch, 0x08))
        {
            const int delayRandom = _rand();
            CSF_AT(float, scratch, 0x04) =
                static_cast<float>(delayRandom) * g_FilmRestartDelayScale +
                g_FilmRestartDelay;

            const int positionRandom = _rand();
            CSF_AT(float, scratch, 0x00) =
                static_cast<float>(positionRandom) * g_FilmRandomScale;

            const int directionRandom = _rand();
            CSF_AT(float, scratch, 0x0C) =
                static_cast<float>(directionRandom) * g_FilmRandomScale <
                        g_LightModulationPeriod
                    ? g_MissingMetricValue
                    : g_TransitionOne;
            CSF_AT(uint8_t, scratch, 0x10) = 40;
            CSF_AT(float, scratch, 0x08) = -1.0f;
        }

        offset += 20;
        --count;
    }

    count = static_cast<int32_t>(self->field_0098);
    if (count != 0)
        count = static_cast<int32_t>(self->field_009c - self->field_0098) / 20;

    offset = 0;
    while (count > 0)
    {
        auto* patch = reinterpret_cast<uint8_t*>(self->field_0098) + offset;

        int random = _rand();
        CSF_AT(float, patch, 0x00) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, patch, 0x04) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, patch, 0x08) =
            static_cast<float>(random) * g_FilmPatchSizeScale + g_FilmPatchSizeBase;
        random = _rand();
        CSF_AT(float, patch, 0x0C) =
            static_cast<float>(random) * g_FilmPatchSizeScale + g_FilmPatchSizeBase;
        random = _rand();
        float alpha = static_cast<float>(random) * g_FilmPatchAlphaScale *
                          g_ColorChannelScale +
                      g_FilmPatchAlphaBase;
        if (alpha > g_ColorChannelScale)
            alpha = g_ColorChannelScale;
        CSF_AT(uint8_t, patch, 0x10) =
            static_cast<uint8_t>(static_cast<int64_t>(alpha));

        offset += 20;
        --count;
    }

    count = self->field_00a8
                ? static_cast<int32_t>(self->field_00ac - self->field_00a8) >> 5
                : 0;

    offset = 0;
    while (count > 0)
    {
        auto* quad = reinterpret_cast<uint8_t*>(self->field_00a8) + offset;

        int random = _rand();
        CSF_AT(float, quad, 0x00) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, quad, 0x04) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, quad, 0x08) =
            static_cast<float>(random) * g_FilmQuadSizeScale + g_FilmSpeckSizeBase;
        random = _rand();
        CSF_AT(float, quad, 0x0C) =
            static_cast<float>(random) * g_FilmQuadSizeScale + g_FilmSpeckSizeBase;
        random = _rand();
        CSF_AT(float, quad, 0x10) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, quad, 0x14) =
            static_cast<float>(random) * g_FilmQuadExtentScale;
        random = _rand();
        CSF_AT(float, quad, 0x18) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, quad, 0x1C) =
            static_cast<float>(random) * g_FilmQuadExtentScale;

        offset += 32;
        --count;
    }

    int result = static_cast<int32_t>(self->field_00b8);
    if (result != 0)
        result = static_cast<int32_t>(self->field_00bc - self->field_00b8) / 20;

    offset = 0;
    while (result > 0)
    {
        auto* speck = reinterpret_cast<uint8_t*>(self->field_00b8) + offset;

        int random = _rand();
        CSF_AT(float, speck, 0x00) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, speck, 0x04) = static_cast<float>(random) * g_FilmRandomScale;
        random = _rand();
        CSF_AT(float, speck, 0x08) =
            static_cast<float>(random) * g_FilmSpeckSizeScale + g_FilmSpeckSizeBase;
        random = _rand();
        CSF_AT(float, speck, 0x0C) =
            static_cast<float>(random) * g_FilmSpeckSizeScale + g_FilmSpeckSizeBase;
        random = _rand();
        float alpha = static_cast<float>(random) * g_FilmSpeckAlphaScale *
                          g_ColorChannelScale +
                      g_FilmSpeckAlphaBase;
        if (alpha > g_ColorChannelScale)
            alpha = g_ColorChannelScale;
        CSF_AT(uint8_t, speck, 0x10) =
            static_cast<uint8_t>(static_cast<int64_t>(alpha));

        offset += 20;
        --result;
    }
    return result;
}

HOOK(0x006FA670, C_00862c74_UpdateFilmNoise, "auto");
