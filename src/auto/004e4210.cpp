#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
// CRT intrinsic form of fmod: it takes both operands on the x87 register stack
// (ST(1) = x, ST(0) = y), which no C++ calling convention can express, so the
// computation below is written as std::fmod.

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);
CSF_GLOBAL(float, g_QueryIntervalScale, 0x0082DF1C);
CSF_GLOBAL(float, g_Const_0082E068, 0x0082E068);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(double, g_LightModulationPeriod, 0x0083E7B0);
CSF_GLOBAL(float, g_LightPhaseRate, 0x0083E7B8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0083e7d8_UpdateLightEffect(
    C_0083e7d8* self, void* /*edx*/, float deltaTime)
{
    void* light = reinterpret_cast<void*>(self->field_0038);
    if (!light)
    {
        self->field_0044 -= deltaTime;
    }
    else
    {
        const float phaseStep = deltaTime * g_LightPhaseRate;
        const double intensity =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, light, 0)[0x40 / 4])(light, nullptr);

        if (intensity > g_QueryIntervalScale)
        {
            // Still too bright: fade the light down.
            light = reinterpret_cast<void*>(self->field_0038);
            void** lightVtable = CSF_AT(void**, light, 0);
            const double currentIntensity =
                ((double(__fastcall*)(void*, void*))
                    lightVtable[0x40 / 4])(light, nullptr);
            const float reducedIntensity =
                static_cast<float>(currentIntensity - phaseStep * g_MaxPercent);

            ((void(__fastcall*)(void*, void*, float))
                lightVtable[0x3C / 4])(
                    reinterpret_cast<void*>(self->field_0038),
                    nullptr, reducedIntensity);
        }
        else if (self->field_0044 > g_fZero)
        {
            // Flicker: modulate the colour with two cosine waves.
            const double firstWave =
                std::cos(static_cast<double>(self->field_003c));
            const double secondWave =
                std::cos(static_cast<double>(self->field_0040));
            const double modulation =
                std::fabs(std::fmod(firstWave + secondWave, g_LightModulationPeriod));

            alignas(8) uint8_t colorBytes[0x18] = {};
            float* color = reinterpret_cast<float*>(colorBytes);
            color[0] = static_cast<float>(g_HalfDurationScale * modulation);
            color[1] = static_cast<float>(g_WheelRecordScale * modulation);
            color[2] = static_cast<float>(g_SoundActivityThreshold * modulation);
            color[3] = 1.0f;

            light = reinterpret_cast<void*>(self->field_0038);
            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, light, 0)[0x34 / 4])(light, nullptr, color);

            self->field_003c = phaseStep * g_SoundActivityThreshold + self->field_003c;
            self->field_0040 = phaseStep * g_Const_0082E068 + self->field_0040;
            self->field_0044 -= deltaTime;
        }
        else
        {
            // Lifetime over: hand the light back to its manager.
            const int globalManager = GetGlobalInt();
            void* clumpManager =
                reinterpret_cast<void*>(ResolveClumpManager(globalManager, nullptr));

            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, clumpManager, 0)[0x2C / 4])(
                    clumpManager, nullptr, self->field_0038);
            self->field_0038 = 0;
        }
    }

    if (self->field_0028 && self->field_0044 < g_fZero)
    {
        CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene),
            nullptr, static_cast<int>(self->field_000c + 28));
    }
}

HOOK(0x004E4210, C_0083e7d8_UpdateLightEffect, "auto");
