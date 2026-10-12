#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SwayHeightScale, 0x0082DC80);
CSF_GLOBAL(float, g_PhaseWrapUnit, 0x00862FE4);
CSF_GLOBAL(float, g_PositivePhaseRate, 0x00862FE8);

static void __fastcall GfxClumpLoader_UpdateWrappedPhase(
    float* self, void* /*edx*/, float delta)
{
    const float rate = CSF_AT(float, self, 0x30) > g_fZero
        ? g_PositivePhaseRate
        : g_SwayHeightScale;

    const float updatedPhase = rate * delta + CSF_AT(float, self, 0xC4);
    CSF_AT(float, self, 0xC4) = updatedPhase;

    if (updatedPhase < g_fZero)
    {
        float phase = CSF_AT(float, self, 0xC4);
        do
        {
            phase += g_PhaseWrapUnit;
        } while (phase < g_fZero);
        CSF_AT(float, self, 0xC4) = phase;
    }

    if (CSF_AT(float, self, 0xC4) > g_PhaseWrapUnit)
    {
        float phase = CSF_AT(float, self, 0xC4);
        do
        {
            phase -= g_PhaseWrapUnit;
        } while (phase > g_PhaseWrapUnit);
        CSF_AT(float, self, 0xC4) = phase;
    }
}

HOOK(0x006FDAF0, GfxClumpLoader_UpdateWrappedPhase, "auto");
