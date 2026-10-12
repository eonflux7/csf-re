#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_FilmBackwardRandomScale, 0x008431AC);
CSF_GLOBAL(float, g_FilmRandomScale, 0x00846234);
CSF_GLOBAL(float, g_FilmForwardOffset, 0x00862AD0);
CSF_GLOBAL(float, g_FilmForwardRandomScale, 0x00862AD4);
CSF_GLOBAL(float, g_FilmBackwardOffset, 0x00862AD8);

static double __stdcall GfxScreenEffects_TransformFilmCoordinate(float coordinate)
{
    const float randomFraction = static_cast<float>(_rand()) * g_FilmRandomScale;
    double result;

    if (_rand() <= 10922)
    {
        result = randomFraction * g_FilmForwardRandomScale
               + coordinate + g_FilmForwardOffset;
        while (result > g_TransitionOne)
            result -= g_TransitionOne;
    }
    else
    {
        result = coordinate - g_FilmBackwardOffset
               - randomFraction * g_FilmBackwardRandomScale;
        while (result < g_TransitionOne)
            result += g_TransitionOne;
    }

    return result;
}

HOOK(0x006F9840, GfxScreenEffects_TransformFilmCoordinate, "auto");
