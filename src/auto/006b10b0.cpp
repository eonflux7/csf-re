#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);
CSF_GLOBAL(float, g_RandomNormalizationScale, 0x00840540);

static char __fastcall CharacterModel_InitializeTransformFromState(
    float* self, void* /*edx*/, int parameter1, int parameter2, int* seed)
{
    CSF_AT(int, self, 0x04) = parameter1;
    CSF_AT(int, self, 0x08) = parameter2;
    CSF_AT(uint8_t, self, 0x00) = 1;

    int nextSeed = 48271 * (*seed % 44488) - 3399 * (*seed / 44488);
    *seed = nextSeed;
    if (nextSeed <= 0)
        *seed = nextSeed + 0x7FFFFFFF;

    CSF_AT(float, self, 0x0C) =
        static_cast<float>(static_cast<double>(*seed) * g_RandomNormalizationScale);

    nextSeed = 48271 * (*seed % 44488) - 3399 * (*seed / 44488);
    *seed = nextSeed;
    if (nextSeed <= 0)
        *seed = nextSeed + 0x7FFFFFFF;

    const double seedValue = static_cast<double>(*seed);
    CSF_AT(uint8_t, self, 0x01) = 1;

    const double normalizedValue = seedValue * g_RandomNormalizationScale;
    CSF_AT(float, self, 0x10) = static_cast<float>(normalizedValue);
    CSF_AT(float, self, 0x14) = static_cast<float>(
        std::sqrt(
            (std::log2(static_cast<double>(g_TransitionOne) - normalizedValue)
             * 0.6931471805599453094)
            * g_MessageBoxSizeScale));

    return 1;
}

HOOK(0x006B10B0, CharacterModel_InitializeTransformFromState, "auto");
