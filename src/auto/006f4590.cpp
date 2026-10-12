#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicUpdate, 0x0057C2D0, (int a1, int a2));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_InitializeEmitterUpdateContext, 0x006F35C0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);

static void __fastcall C_00862748_UpdateParticles(C_00862748* self, void* /*edx*/)
{
    if (self->field_0040 == g_fZero)
        return;

    // Preserve the contiguous frame layout: the update delta is followed
    // by a pointer to the emitter context.
    alignas(8) uint8_t updateBytes[0x38];
    float& delta = CSF_AT(float, updateBytes, 0x28);
    float& lowerBound = CSF_AT(float, updateBytes, 0x2C);
    float& upperBound = CSF_AT(float, updateBytes, 0x30);
    float& step = CSF_AT(float, updateBytes, 0x34);

    int emitter = RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024));
    GfxPrtStdParticles_InitializeEmitterUpdateContext(updateBytes, nullptr, emitter);

    if (self->field_002c != 1)
    {
        void* parameters = reinterpret_cast<void*>(self->field_0030);
        step = CSF_AT(float, parameters, 0x90)
             + CSF_AT(float, parameters, 0x18);

        if (CSF_AT(float, parameters, 0x14) == g_fZero
            && self->field_0040 > step)
        {
            void* emitterState = reinterpret_cast<void*>(
                CSF_AT(uint32_t, updateBytes, 0x00));
            self->field_0040 =
                CSF_AT(float, emitterState, 0x08) < step ? step : g_fZero;
        }

        step = self->field_0040 * g_flt_0082DC84;
        upperBound = 0.3f;
        lowerBound = 0.1f;
        ClampFloatToBounds(&step, &lowerBound, &upperBound);

        for (unsigned int iteration = 0; self->field_0040 > g_fZero; ++iteration)
        {
            if (iteration >= 0xA)
                break;

            delta = self->field_0040 > step ? step : self->field_0040;
            int atomic = static_cast<int>(self->field_0024);
            CSF_AT(uint32_t, updateBytes, 0x2C) =
                reinterpret_cast<uint32_t>(updateBytes);
            RpPrtStdAtomicUpdate(atomic, reinterpret_cast<int>(&delta));
            self->field_0040 = self->field_0040 - step;
        }
    }

    self->field_0040 = 0.0f;
}

HOOK(0x006F4590, C_00862748_UpdateParticles, "auto");
