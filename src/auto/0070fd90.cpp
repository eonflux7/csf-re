#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Audio_UpdateReverbGain, 0x00712AB0, (float a1));
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

struct AudioReverbPreset {
    int32_t room;
    int32_t roomHF;
    float roomRolloff;
    float decayTime;
    float decayHFRatio;
    int32_t reflections;
    float reflectionsDelay;
    int32_t reverb;
    float reverbDelay;
    float diffusion;
    float density;
    float hfReference;
};

static int __fastcall C_00863ba8_SetReverbPreset(
    C_00863ba8* self, void* /*edx*/, int presetIndex)
{
    if (self->field_0020 == static_cast<uint32_t>(presetIndex))
        return static_cast<int>(self->field_0020);

    if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            self->vtbl[0x58 / 4])(self, nullptr))
    {
        const AudioReverbPreset presets[24] = {
            {-10000, -10000, 0.0f, 1.49f, 0.83f, -2602, 0.007f,
             200, 0.011f, 100.0f, 100.0f, 5000.0f},
            {-1000, -100, 0.0f, 1.49f, 0.83f, -2602, 0.007f,
             200, 0.011f, 100.0f, 100.0f, 5000.0f},
            {-1000, -6000, 0.0f, 0.17f, 0.1f, -1204, 0.001f,
             207, 0.002f, 100.0f, 100.0f, 5000.0f},
            {-1000, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f,
             53, 0.003f, 100.0f, 100.0f, 5000.0f},
            {-1000, -1200, 0.0f, 1.49f, 0.54f, -370, 0.007f,
             1030, 0.011f, 100.0f, 60.0f, 5000.0f},
            {-1000, -6000, 0.0f, 0.5f, 0.1f, -1376, 0.003f,
             -1104, 0.004f, 100.0f, 100.0f, 5000.0f},
            {-1000, -300, 0.0f, 2.31f, 0.64f, -711, 0.012f,
             83, 0.017f, 100.0f, 100.0f, 5000.0f},
            {-1000, -476, 0.0f, 4.32f, 0.59f, -789, 0.02f,
             -289, 0.03f, 100.0f, 100.0f, 5000.0f},
            {-1000, -500, 0.0f, 3.92f, 0.7f, -1230, 0.02f,
             -2, 0.029f, 100.0f, 100.0f, 5000.0f},
            {-1000, 0, 0.0f, 2.91f, 1.3f, -602, 0.015f,
             -302, 0.022f, 100.0f, 100.0f, 5000.0f},
            {-1000, -698, 0.0f, 7.24f, 0.33f, -1166, 0.02f,
             16, 0.03f, 100.0f, 100.0f, 5000.0f},
            {-1000, -1000, 0.0f, 10.05f, 0.23f, -602, 0.02f,
             198, 0.03f, 100.0f, 100.0f, 5000.0f},
            {-1000, -4000, 0.0f, 0.3f, 0.1f, -1831, 0.002f,
             -1630, 0.03f, 100.0f, 100.0f, 5000.0f},
            {-1000, -300, 0.0f, 1.49f, 0.59f, -1219, 0.007f,
             441, 0.011f, 100.0f, 100.0f, 5000.0f},
            {-1000, -237, 0.0f, 2.7f, 0.79f, -1214, 0.013f,
             395, 0.02f, 100.0f, 100.0f, 5000.0f},
            {-1000, -270, 0.0f, 1.49f, 0.86f, -1204, 0.007f,
             -4, 0.011f, 100.0f, 100.0f, 5000.0f},
            {-1000, -3300, 0.0f, 1.49f, 0.54f, -2560, 0.162f,
             -613, 0.088f, 79.0f, 100.0f, 5000.0f},
            {-1000, -800, 0.0f, 1.49f, 0.67f, -2273, 0.007f,
             -2217, 0.011f, 50.0f, 100.0f, 5000.0f},
            {-1000, -2500, 0.0f, 1.49f, 0.21f, -2780, 0.3f,
             -2014, 0.1f, 27.0f, 100.0f, 5000.0f},
            {-1000, -1000, 0.0f, 1.49f, 0.83f, -10000, 0.061f,
             500, 0.025f, 100.0f, 100.0f, 5000.0f},
            {-1000, -2000, 0.0f, 1.49f, 0.5f, -2466, 0.179f,
             -2514, 0.1f, 21.0f, 100.0f, 5000.0f},
            {-1000, 0, 0.0f, 1.65f, 1.5f, -1363, 0.008f,
             -1153, 0.012f, 100.0f, 100.0f, 5000.0f},
            {-1000, -1000, 0.0f, 2.81f, 0.14f, 429, 0.014f,
             648, 0.021f, 80.0f, 60.0f, 5000.0f},
            {-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f,
             1700, 0.011f, 100.0f, 100.0f, 5000.0f}
        };
        static_assert(sizeof(AudioReverbPreset) == 0x30);

        std::memcpy(
            reinterpret_cast<uint8_t*>(self->field_0004) + 0x44,
            &presets[presetIndex], 0x30);

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x78)
            |= 0x80u;
    }

    self->field_0020 = static_cast<uint32_t>(presetIndex);
    if (presetIndex == 23)
        return Audio_UpdateReverbGain(self->field_002c * g_ValueScale);

    return Audio_UpdateReverbGain(self->field_002c);
}

HOOK(0x0070FD90, C_00863ba8_SetReverbPreset, "auto");
