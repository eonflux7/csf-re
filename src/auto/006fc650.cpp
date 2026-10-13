#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, S_006b1450_ProcessSceneFrame, 0x006B18A0, (S_006b1450* self, void* , float a2));
CSF_FUNC(char, __fastcall, GfxWater_RenderSceneWithCamera, 0x006B1910, (void* self, void* , int sourceHandle, S_006cc4f0* camera));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, SkinGeom_InitFromSource, 0x006C5A40, (void* self, void* , int sourceHandle, char flagByte));
CSF_FUNC(int, __fastcall, C_008656d0_SetOrClearFlagBits, 0x006C5BE0, (C_008656d0* self, void* , int bitMask, char enable));
CSF_FUNC(char, __fastcall, S_006c5cd0_ResetAndCommitState, 0x006C7A20, (S_006c5cd0* self, void*));
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0, (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxCamera_BeginCameraUpdate, 0x006D3010, (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));

CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);
CSF_GLOBAL(float, g_ColorWhite_A, 0x008A7D9C);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static float LightObject_DecodeFloatStorage(uint32_t storage)
{
    float value;
    std::memcpy(&value, &storage, sizeof(value));
    return value;
}

static int LightObject_EncodeFloatArgument(float value)
{
    int argument;
    std::memcpy(&argument, &value, sizeof(argument));
    return argument;
}

static char __fastcall C_00862e88_Render(C_00862e88* self, void* /*edx*/)
{
    if (!self->field_0054)
        return 1;

    if (!self->field_0010)
    {
        for (int32_t index = 0;
             index < static_cast<int32_t>(self->field_004c); ++index)
        {
            void* child =
                reinterpret_cast<void**>(self->field_0044)[index];
            if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, child, 0)[0x30 / 4])(child, nullptr))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, child, 0)[0x14 / 4])(child, nullptr);
            }
        }
        return 1;
    }

    char enable = (self->field_0040 & 2) == 2;
    auto* state =
        reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    C_008656d0_SetOrClearFlagBits(state, nullptr, 64, enable);

    enable = (self->field_0040 & 8) == 8;
    state = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    C_008656d0_SetOrClearFlagBits(state, nullptr, 128, enable);

    enable = (self->field_0040 & 0x10) == 16;
    state = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    C_008656d0_SetOrClearFlagBits(state, nullptr, 0x8000, enable);

    void* graphicsState =
        reinterpret_cast<void*>(GetGlobalValue_6C5760());
    std::memcpy(CSF_AT(void*, graphicsState, 0x14),
                reinterpret_cast<const void*>(self->field_0038), 0x40);

    alignas(8) uint8_t colorBytes[0x1C];
    float* color = reinterpret_cast<float*>(colorBytes);

    // These camera slots return floating-point values, unlike the
    // generated raster-class wrappers for the same slot numbers.
    void* camera = reinterpret_cast<void*>(self->field_0010);
    CSF_AT(float, colorBytes, 0x10) =
        static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, camera, 0)[0x44 / 4])(camera, nullptr));

    camera = reinterpret_cast<void*>(self->field_0010);
    CSF_AT(float, colorBytes, 0x14) =
        static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, camera, 0)[0x3C / 4])(camera, nullptr));

    if (self->field_0040 & 0x10)
    {
        const float adjusted =
            CSF_AT(float, colorBytes, 0x14) * g_CharacterWidthScale;
        reinterpret_cast<C_008655e8*>(self->field_0010)
            ->vCreateTextureWrapper(LightObject_EncodeFloatArgument(adjusted));
    }

    void* manager =
        reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
    const float intensity = self->field_0034;
    color[0] = g_ColorWhite * intensity;
    color[1] = g_ColorWhite_G * intensity;
    color[2] = g_ColorWhite_B * intensity;
    color[3] = g_ColorWhite_A * intensity;
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, manager, 0)[0x1C / 4])(manager, nullptr, color);

    if (self->field_0040 & 1)
    {
        const float source = LightObject_DecodeFloatStorage(self->field_0010);
        auto* scene = reinterpret_cast<S_006b1450*>(GetGlobalInt());
        S_006b1450_ProcessSceneFrame(scene, nullptr, source);
    }

    int clearMode = 0;
    if (self->field_0040 & 4)
        clearMode = 3;

    char result = GfxCamera_BeginCameraUpdate(
        reinterpret_cast<void*>(self->field_0010), nullptr, clearMode,
        LightObject_DecodeFloatStorage(self->field_0024),
        CSF_AT(float, self, 0x28),
        CSF_AT(float, self, 0x2C),
        static_cast<int>(self->field_0030));
    CSF_AT(char, colorBytes, 0x1B) = result;

    if (result)
    {
        if (self->field_0040 & 1)
        {
            auto* cameraFlag = reinterpret_cast<S_006cc4f0*>(
                static_cast<uintptr_t>(CSF_AT(uint8_t, self, 0x0C)));
            const int sourceHandle = static_cast<int>(self->field_0010);
            void* scene = reinterpret_cast<void*>(GetGlobalInt());
            result = GfxWater_RenderSceneWithCamera(
                scene, nullptr, sourceHandle, cameraFlag);
            CSF_AT(char, colorBytes, 0x1B) = result;
        }

        if (result)
        {
            if (self->field_004c)
            {
                C_008655e8_EndCameraUpdate(
                    reinterpret_cast<C_008655e8*>(self->field_0010), nullptr);

                for (int32_t index = 0;
                     index < static_cast<int32_t>(self->field_004c); ++index)
                {
                    void* child =
                        reinterpret_cast<void**>(self->field_0044)[index];
                    if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, child, 0)[0x30 / 4])(child, nullptr))
                    {
                        reinterpret_cast<void(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, child, 0)[0x14 / 4])(child, nullptr);
                    }
                }

                result = GfxCamera_BeginCameraUpdate(
                    reinterpret_cast<void*>(self->field_0010), nullptr, 0,
                    g_ColorBlack, g_ColorBlack_G, g_ColorBlack_B,
                    LightObject_EncodeFloatArgument(g_ColorBlack_A));
                CSF_AT(char, colorBytes, 0x1B) = result;
            }

            if (result)
            {
                const char flag = static_cast<char>(
                    CSF_AT(uint8_t, self, 0x0C));
                const int sourceHandle = static_cast<int>(self->field_0010);
                graphicsState =
                    reinterpret_cast<void*>(GetGlobalValue_6C5760());
                SkinGeom_InitFromSource(
                    graphicsState, nullptr, sourceHandle, flag);

                for (uint32_t index = 0; index < self->field_0060; ++index)
                {
                    void* child =
                        reinterpret_cast<void**>(self->field_0058)[index];
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, child, 0)[0x14 / 4])(child, nullptr);
                }

                auto* commitState =
                    reinterpret_cast<S_006c5cd0*>(GetGlobalValue_6C5760());
                const char committed =
                    S_006c5cd0_ResetAndCommitState(commitState, nullptr);
                CSF_AT(char, colorBytes, 0x1B) &= committed;

                C_008655e8_EndCameraUpdate(
                    reinterpret_cast<C_008655e8*>(self->field_0010), nullptr);
            }
        }
    }

    manager =
        reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, manager, 0)[0x1C / 4])(
            manager, nullptr, &g_ColorBlack);

    reinterpret_cast<C_008655e8*>(self->field_0010)
        ->vCreateResourceWrapper(LightObject_EncodeFloatArgument(
            CSF_AT(float, colorBytes, 0x10)));
    reinterpret_cast<C_008655e8*>(self->field_0010)
        ->vCreateTextureWrapper(LightObject_EncodeFloatArgument(
            CSF_AT(float, colorBytes, 0x14)));

    return CSF_AT(char, colorBytes, 0x1B);
}

HOOK(0x006FC650, C_00862e88_Render, "auto");
