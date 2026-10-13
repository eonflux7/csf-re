#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(char, __cdecl, GfxSkin_ComputeDistanceFade, 0x006C5310, (float* fade, float* position, float* referencePosition, float distanceScale, float maximumDistance, float minimumDistance, float fadeWidth));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_00862490_CheckSpatialVisibility, 0x006F1CB0, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(char, __fastcall, C_00862490_CheckOcclusion, 0x006F1F00, (void* self, void* edx, int a2, unsigned int a3));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_DistanceCullThresholdSquared, 0x00862510);

static char __fastcall C_00862490_UpdateVisibilityAndFade(
    C_00862490* self, void* /*edx*/)
{
    auto* referencePosition = reinterpret_cast<float*>(
        static_cast<uintptr_t>(GetGlobalValue_6C5760()) + 292);

    void* positionProvider = reinterpret_cast<void*>(self->field_0020);
    void* transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, positionProvider, 0)[0x14 / 4])(
                positionProvider, nullptr);
    float* position =
        ((float* (__fastcall*)(void*, void*))
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    void* settings = reinterpret_cast<void*>(self->field_0018);
    float* extent = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(settings) + 0x4C);
    if (!(CSF_AT(float, settings, 0x50) < CSF_AT(float, settings, 0x4C)))
        extent = reinterpret_cast<float*>(
            reinterpret_cast<uint8_t*>(settings) + 0x50);

    alignas(8) uint8_t sphereBytes[0x24];
    CSF_AT(uint32_t, sphereBytes, 0x00) = CSF_AT(uint32_t, position, 0x00);
    CSF_AT(uint32_t, sphereBytes, 0x04) = CSF_AT(uint32_t, position, 0x04);
    CSF_AT(uint32_t, sphereBytes, 0x08) = CSF_AT(uint32_t, position, 0x08);
    CSF_AT(float, sphereBytes, 0x0C) = *extent * g_HalfDurationScale;

    void* globalState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    void* visibilityQuery = CSF_AT(void*, globalState, 0xF0);
    char visible =
        ((char (__fastcall*)(void*, void*, void*, uint32_t*, int))
            CSF_AT(void**, visibilityQuery, 0)[0x6C / 4])(
                visibilityQuery, nullptr, sphereBytes, &self->field_0044, 0);

    if (visible)
    {
        settings = reinterpret_cast<void*>(self->field_0018);
        float minimumDistance = CSF_AT(float, settings, 0x6C);
        float maximumDistance = CSF_AT(float, settings, 0x68);
        float fadeWidth = CSF_AT(float, settings, 0x70);
        visible = GfxSkin_ComputeDistanceFade(
            &self->field_003c, position, referencePosition,
            CSF_AT(float, referencePosition, 0x0C),
            maximumDistance, minimumDistance, fadeWidth);
    }

    if (!visible)
    {
        void* transition = reinterpret_cast<void*>(self->field_0040);
        float currentValue = static_cast<float>(
            ((double (__fastcall*)(void*, void*))
                CSF_AT(void**, transition, 0)[0x08 / 4])(
                    transition, nullptr));
        if (CSF_AT(uint8_t, referencePosition, 0x10)
            && !self->field_004c && currentValue != g_fZero)
        {
            transition = reinterpret_cast<void*>(self->field_0040);
            ((void (__fastcall*)(void*, void*, float))
                CSF_AT(void**, transition, 0)[0x04 / 4])(
                    transition, nullptr, 0.0f);
        }
        return visible;
    }

    if (CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x60)
        != g_fZero)
    {
        uint32_t referenceAddress =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(referencePosition));
        float referenceArgument;
        std::memcpy(&referenceArgument, &referenceAddress, sizeof(referenceArgument));
        visible = static_cast<char>(C_00862490_CheckSpatialVisibility(
            self, nullptr, position, referenceArgument));
    }

    if (visible && !self->field_004c)
    {
        char occluded = C_00862490_CheckOcclusion(
            self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(position)),
            static_cast<unsigned int>(reinterpret_cast<uintptr_t>(referencePosition)));

        if (!CSF_AT(uint8_t, referencePosition, 0x10))
        {
            visible = occluded == 0;
        }
        else
        {
            void* transition = reinterpret_cast<void*>(self->field_0040);
            float currentValue = static_cast<float>(
                ((double (__fastcall*)(void*, void*))
                    CSF_AT(void**, transition, 0)[0x08 / 4])(
                        transition, nullptr));

            if (occluded)
            {
                if (currentValue != g_fZero)
                {
                    transition = reinterpret_cast<void*>(self->field_0040);
                    bool active =
                        ((bool (__fastcall*)(void*, void*))
                            CSF_AT(void**, transition, 0)[0x1C / 4])(
                                transition, nullptr);
                    if (!active
                        || CSF_AT(float,
                            reinterpret_cast<void*>(self->field_0040), 0x0C)
                            != g_fZero)
                    {
                        float duration = CSF_AT(float,
                            reinterpret_cast<void*>(self->field_0018), 0x64);
                        transition = reinterpret_cast<void*>(self->field_0040);
                        ((void (__fastcall*)(void*, void*, float, float))
                            CSF_AT(void**, transition, 0)[0x0C / 4])(
                                transition, nullptr, 0.0f, duration);
                    }
                }
            }
            else if (currentValue != g_TransitionOne)
            {
                transition = reinterpret_cast<void*>(self->field_0040);
                bool active =
                    ((bool (__fastcall*)(void*, void*))
                        CSF_AT(void**, transition, 0)[0x1C / 4])(
                            transition, nullptr);
                if (!active
                    || CSF_AT(float,
                        reinterpret_cast<void*>(self->field_0040), 0x0C)
                        != g_TransitionOne)
                {
                    float duration = CSF_AT(float,
                        reinterpret_cast<void*>(self->field_0018), 0x64);
                    transition = reinterpret_cast<void*>(self->field_0040);
                    ((void (__fastcall*)(void*, void*, float, float))
                        CSF_AT(void**, transition, 0)[0x0C / 4])(
                            transition, nullptr, 1.0f, duration);
                }
            }
            visible = currentValue != g_fZero;
        }
    }

    if (!CSF_AT(uint8_t, referencePosition, 0x10))
        return visible;

    float dx = CSF_AT(float, position, 0x00)
             - CSF_AT(float, referencePosition, 0x00);
    float dy = CSF_AT(float, position, 0x04)
             - CSF_AT(float, referencePosition, 0x04);
    float dz = CSF_AT(float, position, 0x08)
             - CSF_AT(float, referencePosition, 0x08);
    uint8_t distanceCulled =
        (dz * dz + dy * dy + dx * dx)
        * CSF_AT(float, referencePosition, 0x0C)
        * CSF_AT(float, referencePosition, 0x0C)
        > g_DistanceCullThresholdSquared;

    bool changed = self->field_004c != distanceCulled;
    if (changed)
    {
        self->field_004c = distanceCulled;
        void* item = reinterpret_cast<void*>(
            static_cast<uintptr_t>(GetCurrentItemCount(self, nullptr)));
        unsigned int keyedValue = distanceCulled != 0;
        void* keyedValues =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, item, 0)[0x1C / 4])(item, nullptr);
        GfxSkin_SetKeyedValue(keyedValues, nullptr, 6, keyedValue);
    }

    if (!changed)
        return visible;

    if (distanceCulled)
    {
        void* transition = reinterpret_cast<void*>(self->field_0040);
        ((void (__fastcall*)(void*, void*, float))
            CSF_AT(void**, transition, 0)[0x04 / 4])(
                transition, nullptr, 1.0f);
        return visible;
    }

    char occluded = C_00862490_CheckOcclusion(
        self, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(position)),
        static_cast<unsigned int>(reinterpret_cast<uintptr_t>(referencePosition)));
    char newlyVisible = occluded == 0;
    float newValue = occluded ? 0.0f : 1.0f;
    void* transition = reinterpret_cast<void*>(self->field_0040);
    ((void (__fastcall*)(void*, void*, float))
        CSF_AT(void**, transition, 0)[0x04 / 4])(
            transition, nullptr, newValue);
    return newlyVisible;
}

HOOK(0x006F2150, C_00862490_UpdateVisibilityAndFade, "auto");
