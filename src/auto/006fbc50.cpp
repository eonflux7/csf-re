#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplyEffectPass, 0x00708DE0, (void* self, void* edx, int a2, char a3));
CSF_FUNC(char, __fastcall, ScreenEffects_ApplyLayerColor, 0x00708E40, (void* self, void* edx, int a2, int a3, int a4, char a5, int a6, float* a7));
CSF_FUNC(int, __stdcall, ScreenEffects_PrepareLayer, 0x00709360, (int a1));

CSF_GLOBAL(float, g_MaxVehicleTimeStep, 0x0082E26C);
CSF_GLOBAL(float, g_LayerOffsetScale, 0x00862D0C);

static int __fastcall C_00862d48_ApplyFilmLayers(
    C_00862d48* self, void* /*edx*/)
{
    if (((bool(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr))
    {
        void* accessor = reinterpret_cast<void*>(GetGlobalInt());
        GlobalInt_ResolveAccessor(accessor, nullptr);
        SceneLoader_NoOp();
        ScreenEffects_PrepareLayer(static_cast<int>(self->field_0060));

        alignas(8) uint8_t originBytes[0x8] = {};
        alignas(8) uint8_t startBytes[0xC] = {};
        alignas(8) uint8_t endBytes[0x8] = {};
        alignas(8) uint8_t repeatBytes[0x10] = {};

        for (int layer = 1; layer < 5; ++layer)
        {
            CSF_AT(uint8_t, repeatBytes, 0xF) = 0xB4;
            CSF_AT(int, startBytes, 0x8) = self->vSlot20(2);
            CSF_AT(float, repeatBytes, 0x8) =
                static_cast<float>(CSF_AT(int, startBytes, 0x8)) *
                g_MaxVehicleTimeStep;
            if ((layer & 1) != 0)
                CSF_AT(float, repeatBytes, 0x8) =
                    -CSF_AT(float, repeatBytes, 0x8);

            const uint8_t intensity = layer == 1
                ? static_cast<uint8_t>(self->vSlot20(0))
                : CSF_AT(uint8_t, repeatBytes, 0xF);

            int destination = CSF_AT(int, self, 0x60 + 4 * layer);
            CSF_AT(float, startBytes, 0x4) =
                CSF_AT(float, repeatBytes, 0x8) * g_LayerOffsetScale;
            const int color =
                intensity | ((intensity | (intensity << 8)) << 8);
            int source = CSF_AT(int, self, 0x5C + 4 * layer);
            CSF_AT(float, startBytes, 0) = 0.0f;
            ScreenEffects_ApplyLayerColor(
                self, nullptr, source, destination, color,
                static_cast<char>(255), 1,
                reinterpret_cast<float*>(startBytes));

            destination = CSF_AT(int, self, 0x60 + 4 * layer);
            const float negativeOffset = -CSF_AT(float, repeatBytes, 0x8);
            source = CSF_AT(int, self, 0x5C + 4 * layer);
            CSF_AT(float, endBytes, 0) = negativeOffset;
            CSF_AT(float, startBytes, 0x8) = negativeOffset;
            CSF_AT(float, endBytes, 0x4) = negativeOffset;
            ScreenEffects_ApplyLayerColor(
                self, nullptr, source, destination, color,
                static_cast<char>(255), 0,
                reinterpret_cast<float*>(endBytes));

            CSF_AT(float, repeatBytes, 0) =
                CSF_AT(float, repeatBytes, 0x8);
            CSF_AT(int, repeatBytes, 0x4) = CSF_AT(int, startBytes, 0x8);
            destination = CSF_AT(int, self, 0x60 + 4 * layer);
            source = CSF_AT(int, self, 0x5C + 4 * layer);
            ScreenEffects_ApplyLayerColor(
                self, nullptr, source, destination, color,
                static_cast<char>(255), 0,
                reinterpret_cast<float*>(repeatBytes));
        }

        uint8_t* layerAddress = reinterpret_cast<uint8_t*>(self) + 0x70;
        CSF_AT(uint8_t, repeatBytes, 0x4) = 1;
        int layersRemaining = 2;
        do
        {
            int remainingIntensity = self->vSlot20(1);
            CSF_AT(int, endBytes, 0x4) = remainingIntensity;
            if (remainingIntensity != 0)
            {
                CSF_AT(int, startBytes, 0x4) = 255;
                do
                {
                    const uint8_t intensity = remainingIntensity >= 255
                        ? CSF_AT(uint8_t, startBytes, 0x4)
                        : CSF_AT(uint8_t, endBytes, 0x4);
                    const int flags = CSF_AT(int, repeatBytes, 0x4);
                    const int color =
                        intensity | ((intensity | (intensity << 8)) << 8);
                    const int destination = static_cast<int>(self->field_0060);
                    const int source = CSF_AT(int, layerAddress, 0);
                    CSF_AT(float, originBytes, 0) = 0.0f;
                    CSF_AT(float, originBytes, 0x4) = 0.0f;
                    ScreenEffects_ApplyLayerColor(
                        self, nullptr, source, destination, color,
                        static_cast<char>(255), flags,
                        reinterpret_cast<float*>(originBytes));

                    remainingIntensity = static_cast<int32_t>(
                        static_cast<uint32_t>(remainingIntensity) - intensity);
                    CSF_AT(uint8_t, repeatBytes, 0x4) = 0;
                    CSF_AT(int, endBytes, 0x4) = remainingIntensity;
                } while (remainingIntensity != 0);
            }
            layerAddress -= 4;
            --layersRemaining;
        } while (layersRemaining != 0);

        ApplyEffectPass(
            self, nullptr, static_cast<int>(self->field_0060),
            static_cast<char>(255));
    }

    return ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x3C / 4])(self, nullptr);
}

HOOK(0x006FBC50, C_00862d48_ApplyFilmLayers, "auto");
