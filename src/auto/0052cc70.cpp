#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, Hud_ColorsMatchRgba, 0x0042D680, (float* self, void* , float* other));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __stdcall MenuProgressInput_UpdateBlinkColor(int outputAddress, int animationAddress)
{
    float color[4] = {
        g_ColorBlack, g_ColorBlack_G, g_ColorBlack_B, g_ColorBlack_A
    };

    auto* animation = reinterpret_cast<uint8_t*>(animationAddress);
    auto* output = reinterpret_cast<uint8_t*>(outputAddress);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    CSF_AT(float, animation, 0x28) =
        CSF_AT(float, state, 0x9C) + CSF_AT(float, animation, 0x28);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* timingOwner = CSF_AT(void*, state, 0x14);
    void* timing = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, timingOwner, 0)[0x54 / 4])(timingOwner, nullptr);
    float* duration = reinterpret_cast<float* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, timing, 0)[0x48 / 4])(timing, nullptr);
    const float halfDuration = *duration * g_HalfDurationScale;

    if (CSF_AT(float, animation, 0x28) >= halfDuration)
    {
        if (Hud_ColorsMatchRgba(reinterpret_cast<float*>(animation + 0x3C),
                               nullptr,
                               reinterpret_cast<float*>(animation + 0x18)))
        {
            CSF_AT(uint32_t, animation, 0x2C) = CSF_AT(uint32_t, animation, 0x18);
            CSF_AT(uint32_t, animation, 0x30) = CSF_AT(uint32_t, animation, 0x1C);
            CSF_AT(uint32_t, animation, 0x34) = CSF_AT(uint32_t, animation, 0x20);
            CSF_AT(uint32_t, animation, 0x38) = CSF_AT(uint32_t, animation, 0x24);

            CSF_AT(uint32_t, animation, 0x3C) = CSF_AT(uint32_t, animation, 0x08);
            CSF_AT(uint32_t, animation, 0x40) = CSF_AT(uint32_t, animation, 0x0C);
            CSF_AT(uint32_t, animation, 0x44) = CSF_AT(uint32_t, animation, 0x10);
            CSF_AT(uint32_t, animation, 0x48) = CSF_AT(uint32_t, animation, 0x14);
        }
        else if (Hud_ColorsMatchRgba(reinterpret_cast<float*>(animation + 0x3C),
                                    nullptr,
                                    reinterpret_cast<float*>(animation + 0x08)))
        {
            CSF_AT(uint32_t, animation, 0x2C) = CSF_AT(uint32_t, animation, 0x08);
            CSF_AT(uint32_t, animation, 0x30) = CSF_AT(uint32_t, animation, 0x0C);
            CSF_AT(uint32_t, animation, 0x34) = CSF_AT(uint32_t, animation, 0x10);
            CSF_AT(uint32_t, animation, 0x38) = CSF_AT(uint32_t, animation, 0x14);

            CSF_AT(uint32_t, animation, 0x3C) = CSF_AT(uint32_t, animation, 0x18);
            CSF_AT(uint32_t, animation, 0x40) = CSF_AT(uint32_t, animation, 0x1C);
            CSF_AT(uint32_t, animation, 0x44) = CSF_AT(uint32_t, animation, 0x20);
            CSF_AT(uint32_t, animation, 0x48) = CSF_AT(uint32_t, animation, 0x24);
        }

        CSF_AT(float, animation, 0x28) -= halfDuration;
    }

    const float fraction = CSF_AT(float, animation, 0x28) / halfDuration;
    color[3] = 255.0f;

    const float red =
        (CSF_AT(float, animation, 0x3C) - CSF_AT(float, animation, 0x2C)) *
        fraction + CSF_AT(float, animation, 0x2C);
    const float green =
        (CSF_AT(float, animation, 0x40) - CSF_AT(float, animation, 0x30)) *
        fraction + CSF_AT(float, animation, 0x30);
    const float blue =
        (CSF_AT(float, animation, 0x44) - CSF_AT(float, animation, 0x34)) *
        fraction + CSF_AT(float, animation, 0x34);

    color[0] = red * g_ColorChannelScale;
    CSF_AT(float, output, 0x00) = color[0];
    color[1] = green * g_ColorChannelScale;
    CSF_AT(float, output, 0x04) = color[1];
    color[2] = blue * g_ColorChannelScale;
    CSF_AT(float, output, 0x08) = color[2];
    CSF_AT(float, output, 0x0C) = color[3];

    return outputAddress;
}

HOOK(0x0052CC70, MenuProgressInput_UpdateBlinkColor, "auto");
