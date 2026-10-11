#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharacterModel_SyncCachedTransform, 0x004F5850,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, CharacterModel_EvaluateMotionChannel, 0x006B1150,
         (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MotionThreshold, 0x0083F188);
CSF_GLOBAL(float, g_MinimumMotionDuration, 0x0083F194);
CSF_GLOBAL(uint8_t, g_MotionPhase, 0x008998F8);

static void __fastcall CharacterModel_UpdateMotion(void* self, void* /*edx*/,
                                                   float deltaTime)
{
    if (CSF_AT(float, self, 0x24) < g_MotionThreshold) {
        CSF_AT(float, self, 0x60) = 0.0f;
        CSF_AT(int32_t, self, 0x70) = 2139095039;
        CSF_AT(int32_t, self, 0x6C) = 2139095039;
        CSF_AT(int32_t, self, 0x68) = 2139095039;
        CSF_AT(int32_t, self, 0x64) = 2139095039;
        g_MotionPhase = 1;
        return;
    }

    float duration = 0.000002f;
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (g_MinimumMotionDuration < CSF_AT(float, settings, 0x9CC))
        duration = CSF_AT(float, settings, 0x9CC);

    float remaining = CSF_AT(float, self, 0x60) - deltaTime;
    CSF_AT(float, self, 0x60) = remaining;

    if (remaining < g_MotionThreshold) {
        if (g_MotionPhase) {
            auto* state = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
            double first = CharacterModel_EvaluateMotionChannel(
                static_cast<uint8_t*>(self) + 0x40, nullptr,
                reinterpret_cast<int*>(state + 0xC4));
            CSF_AT(float, self, 0x58) =
                static_cast<float>(first * CSF_AT(float, self, 0x24));

            state = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
            double second = CharacterModel_EvaluateMotionChannel(
                static_cast<uint8_t*>(self) + 0x28, nullptr,
                reinterpret_cast<int*>(state + 0xC4));
            float secondOffset =
                static_cast<float>(second * CSF_AT(float, self, 0x24));
            g_MotionPhase = 0;
            CSF_AT(float, self, 0x5C) = secondOffset;
        } else {
            g_MotionPhase = 1;
        }
        CSF_AT(float, self, 0x60) = duration;
    }

    float weight = CSF_AT(float, self, 0x60) / duration;
    if (!g_MotionPhase)
        weight = g_TransitionOne - weight;

    CharacterModel_SyncCachedTransform(self, nullptr);

    void* parent = CSF_AT(void*, self, 0x04);
    void* transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    float firstOffset = weight * CSF_AT(float, self, 0x58);
    ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, transform, 0)[0x3C / 4])(
        transform, nullptr, firstOffset);

    parent = CSF_AT(void*, self, 0x04);
    transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    float secondOffset = weight * CSF_AT(float, self, 0x5C);
    ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, transform, 0)[0x40 / 4])(
        transform, nullptr, secondOffset);

    parent = CSF_AT(void*, self, 0x04);
    transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    CSF_AT(float, self, 0x6C) =
        ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x24 / 4])(
            transform, nullptr);

    parent = CSF_AT(void*, self, 0x04);
    transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    CSF_AT(float, self, 0x70) =
        ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x28 / 4])(
            transform, nullptr);
}

HOOK(0x004F6990, CharacterModel_UpdateMotion, "auto");
