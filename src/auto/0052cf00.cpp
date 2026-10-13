#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(void, __fastcall, MenuProgressInput_AdvanceTweenStep, 0x0052CAF0, (void* self, void* , float timeStep));
CSF_FUNC(int, __stdcall, MenuProgressInput_UpdateBlinkColor, 0x0052CC70, (int outputAddress, int animationAddress));
CSF_FUNC(int, __fastcall, MenuProgressInput_ResetTextInputLayout, 0x0052CDF0, (void* self, void*));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __fastcall C_008414e8_UpdateAnimations(
    C_008414e8* self, void* /*edx*/, float timeStep)
{
    std::memcpy(&self->field_0040, &timeStep, sizeof(timeStep));

    int deltaBits;
    std::memcpy(&deltaBits, &timeStep, sizeof(deltaBits));
    auto* timeState =
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    const int mode = CSF_AT(int, self, 0x20);
    if (mode == 0)
        MenuProgressInput_AdvanceTweenStep(self, nullptr, timeStep);
    else if (mode == 1)
        MenuProgressInput_ResetTextInputLayout(self, nullptr);

    alignas(8) uint8_t colorBytes[0x1C];
    uint32_t* controlSlot = &self->field_0024;
    uint8_t* colorCursor = reinterpret_cast<uint8_t*>(self) + 0x64;

    for (int remaining = 3; remaining != 0; --remaining)
    {
        if (g_ColorBlack != CSF_AT(float, colorCursor, -0x8) ||
            g_ColorBlack_G != CSF_AT(float, colorCursor, -0x4) ||
            g_ColorBlack_B != CSF_AT(float, colorCursor, 0) ||
            g_ColorBlack_A != CSF_AT(float, colorCursor, 0x4))
        {
            void* control = reinterpret_cast<void*>(*controlSlot);
            void* colorTarget =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, control, 0)[0x110 / 4])(
                        control, nullptr);

            void** targetVtable = CSF_AT(void**, colorTarget, 0);
            const int updatedColor = MenuProgressInput_UpdateBlinkColor(
                reinterpret_cast<int>(colorBytes),
                reinterpret_cast<int>(colorCursor - 0x20));

            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                targetVtable[0x60 / 4])(
                    colorTarget, nullptr, updatedColor);
        }

        ++controlSlot;
        colorCursor += 0x4C;
    }

    return 0;
}

HOOK(0x0052CF00, C_008414e8_UpdateAnimations, "auto");
