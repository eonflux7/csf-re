#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __stdcall, GfxScreenEffects_TransformFilmCoordinate, 0x006F9840, (float a1));
CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(float, g_WheelTravelMultiplier, 0x0082E24C);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);
CSF_GLOBAL(float, g_FilmRandomScale, 0x00846234);

static int __fastcall C_00862b04_UpdateFilmCoordinates(
    C_00862b04* self, void* /*edx*/)
{
    for (int index = 0; index < 4; ++index)
    {
        const int coordinateOffset = 0x78 + index * 4;
        const int outputOffset = 0x8C + index * 0x10;

        CSF_AT(float, self, outputOffset - 4) =
            static_cast<float>(_rand()) * g_FilmRandomScale + g_RotationWrapStep;

        const int randomValue = _rand();
        const float firstCoordinate = CSF_AT(float, self, coordinateOffset - 0x10);
        CSF_AT(float, self, outputOffset) =
            static_cast<float>(randomValue) * g_FilmRandomScale *
            g_ControlHoldThreshold + g_WheelTravelMultiplier;

        const double firstResult =
            GfxScreenEffects_TransformFilmCoordinate(firstCoordinate);
        const float secondCoordinate = CSF_AT(float, self, coordinateOffset);
        CSF_AT(float, self, coordinateOffset - 0x10) = static_cast<float>(firstResult);
        CSF_AT(float, self, outputOffset + 4) = static_cast<float>(firstResult);

        const double secondResult =
            GfxScreenEffects_TransformFilmCoordinate(secondCoordinate);
        CSF_AT(float, self, coordinateOffset) = static_cast<float>(secondResult);
        CSF_AT(float, self, outputOffset + 8) = static_cast<float>(secondResult);
    }
    return 0;
}

HOOK(0x006F98F0, C_00862b04_UpdateFilmCoordinates, "auto");
