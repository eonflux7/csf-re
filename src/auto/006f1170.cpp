#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static void __fastcall C_00862390_UpdateFlickeringColor(
    C_00862390* self, void* /*edx*/, float elapsedTime)
{
    void* curve = reinterpret_cast<void*>(self->field_0034);
    if (!curve)
        return;

    elapsedTime += self->field_0038;
    self->field_0038 = elapsedTime;

    if (((bool(__fastcall*)(void*, void*, float))
             CSF_AT(void**, curve, 0)[0x10 / 4])(
            curve, nullptr, elapsedTime))
    {
        curve = reinterpret_cast<void*>(self->field_0034);
        void** curveVtable = CSF_AT(void**, curve, 0);
        int count = ((int(__fastcall*)(void*, void*))
                         curveVtable[0x20 / 4])(curve, nullptr);

        double lastTime = ((double(__fastcall*)(void*, void*, int))
                               curveVtable[0x28 / 4])(
            reinterpret_cast<void*>(self->field_0034), nullptr, count - 1);

        void* settings = reinterpret_cast<void*>(self->field_0018);
        self->field_0038 = self->field_0038 - lastTime;

        unsigned int randomValue = RtRandom() % 0x3E8u;
        curve = reinterpret_cast<void*>(self->field_0034);
        float spacing =
            (static_cast<float>(randomValue) * g_SceneLoadMillisecondsToSeconds *
                 CSF_AT(float, settings, 0x6C) +
             CSF_AT(float, settings, 0x68)) *
            g_SoundActivityThreshold;

        curveVtable = CSF_AT(void**, curve, 0);
        count = ((int(__fastcall*)(void*, void*))
                     curveVtable[0x20 / 4])(curve, nullptr);
        float* lastValue = ((float*(__fastcall*)(void*, void*, int))
                               curveVtable[0x24 / 4])(
            reinterpret_cast<void*>(self->field_0034), nullptr, count - 1);

        elapsedTime = *lastValue;
        curve = reinterpret_cast<void*>(self->field_0034);
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, curve, 0)[0x1C / 4])(curve, nullptr);

        curve = reinterpret_cast<void*>(self->field_0034);
        ((void(__fastcall*)(void*, void*, float*, float))
             CSF_AT(void**, curve, 0)[0x18 / 4])(
            curve, nullptr, &elapsedTime, 0.0f);

        float amplitude = 1.0f;
        for (unsigned int i = 1; i <= 5; ++i)
        {
            amplitude =
                (g_TransitionOne -
                 CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x70)) *
                amplitude;

            randomValue = RtRandom() % 0x3E8u;
            float randomFraction =
                static_cast<float>(randomValue) * g_SceneLoadMillisecondsToSeconds;
            elapsedTime +=
                (randomFraction + randomFraction - g_TransitionOne) * amplitude;

            if (elapsedTime < g_fZero)
                elapsedTime = 0.0f;
            else if (elapsedTime > g_ControlHoldThreshold)
                elapsedTime = 1.5f;

            curve = reinterpret_cast<void*>(self->field_0034);
            float sampleTime = static_cast<float>(i) * spacing;
            ((void(__fastcall*)(void*, void*, float*, float))
                 CSF_AT(void**, curve, 0)[0x18 / 4])(
                curve, nullptr, &elapsedTime, sampleTime);
        }
    }

    curve = reinterpret_cast<void*>(self->field_0034);
    double blend = ((double(__fastcall*)(void*, void*, float))
                        CSF_AT(void**, curve, 0)[0x14 / 4])(
        curve, nullptr, self->field_0038);

    void* settings = reinterpret_cast<void*>(self->field_0018);
    float inverseBlend = g_TransitionOne - blend;

    alignas(8) uint8_t colorBytes[0x1C];
    float* color = reinterpret_cast<float*>(colorBytes);
    color[3] = 1.0f;
    color[0] =
        (CSF_AT(uint8_t, settings, 0x4C) * blend +
         CSF_AT(uint8_t, settings, 0x61) * inverseBlend) *
        g_fInv255;
    color[1] =
        (CSF_AT(uint8_t, settings, 0x4D) * blend +
         CSF_AT(uint8_t, settings, 0x62) * inverseBlend) *
        g_fInv255;
    color[2] =
        (CSF_AT(uint8_t, settings, 0x4E) * blend +
         CSF_AT(uint8_t, settings, 0x63) * inverseBlend) *
        g_fInv255;

    void* colorTarget = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, float*))
         CSF_AT(void**, colorTarget, 0)[0x34 / 4])(
        colorTarget, nullptr, color);
}

HOOK(0x006F1170, C_00862390_UpdateFlickeringColor, "auto");
