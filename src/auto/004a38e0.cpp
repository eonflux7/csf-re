#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008344f0_ResetAnimationSoundTimers, 0x004A3670, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_008344f0_UpdateAnimationSounds(
    C_008344f0* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0048 == UINT32_MAX || self->field_004c == UINT32_MAX)
        return;

    alignas(8) uint8_t requestBytes[0x38];
    alignas(8) uint8_t listenerBytes[0x18];
    S_00526d40& request = *reinterpret_cast<S_00526d40*>(requestBytes);

    void* setup = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC4));
    const int animationEntry = SceneMissionSetup_GetAnimationEntry(
        setup, nullptr, static_cast<int>(self->field_0048));
    CSF_AT(int, listenerBytes, 0x10) = animationEntry;

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    void* animationController = reinterpret_cast<void*>(owner->vGetField016c());
    void* playback = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, animationController, 0)[0x68 / 4])(
            animationController, nullptr);
    const float animationTime = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, playback, 0)[0x34 / 4])(playback, nullptr, 0));

    CSF_AT(float, listenerBytes, 0x14) = animationTime;
    if (animationTime < self->field_0060)
    {
        C_008344f0_ResetAnimationSoundTimers(self, nullptr);
        elapsedTime = CSF_AT(float, listenerBytes, 0x14);
    }

    int32_t timerCount = static_cast<int32_t>(self->field_0058);
    self->field_0060 = CSF_AT(float, listenerBytes, 0x14);
    CSF_AT(int32_t, listenerBytes, 0x14) = timerCount;

    for (int timerIndex = 0; timerIndex < timerCount; ++timerIndex)
    {
        float* timer = reinterpret_cast<float*>(self->field_0050) + timerIndex;

        // The original also enters this branch for an unordered comparison.
        if (!(*timer < g_fZero))
        {
            *timer = *timer - elapsedTime;
            if (reinterpret_cast<float*>(self->field_0050)[timerIndex] < g_fZero)
            {
                CSF_AT(int, requestBytes, 0x28) = -1;
                CSF_AT(int, requestBytes, 0x2C) = -1;
                CSF_AT(int, requestBytes, 0x08) = -1;
                CSF_AT(int, requestBytes, 0x0C) = -1;
                owner = reinterpret_cast<C_00833908*>(self->field_0008);
                CSF_AT(int, requestBytes, 0x18) = 0;
                CSF_AT(int, requestBytes, 0x1C) = 0;
                CSF_AT(int, requestBytes, 0x20) = 0;
                CSF_AT(void**, requestBytes, 0x24) = CSF_VTBL(0x0082D448);
                request.field_0000 = 1;
                CSF_AT(int, requestBytes, 0x04) = 1;
                CSF_AT(int, requestBytes, 0x10) = 0;
                CSF_AT(int, requestBytes, 0x14) = 0;
                CSF_AT(uint8_t, requestBytes, 0x34) = 0;
                CSF_AT(uint8_t, requestBytes, 0x35) = 1;
                CSF_AT(uint8_t, requestBytes, 0x36) = 1;
                CSF_AT(int, requestBytes, 0x30) = 0;

                if (owner->field_0050 ||
                    reinterpret_cast<C_00833908*>(
                        GetSceneMenuContext(g_pCurrentScene, nullptr)) != owner)
                {
                    request.field_0000 = 2;
                    CSF_AT(int, requestBytes, 0x04) = 2;
                }
                else
                {
                    request.field_0000 = 4;
                    CSF_AT(int, requestBytes, 0x04) = 3;
                }

                const uint32_t entryOffset = 44 * self->field_004c;
                void* entries = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(CSF_AT(int, listenerBytes, 0x10)),
                        0x24));
                auto* soundEntry = reinterpret_cast<uint8_t*>(entries) + entryOffset;
                void* sounds = reinterpret_cast<void*>(
                    CSF_AT(uint32_t, soundEntry, 0x20));

                if (CSF_AT(int, sounds, 8 * timerIndex) == -2)
                {
                    if ((owner->field_0170 & 0x2000) != 0)
                    {
                        auto* character =
                            reinterpret_cast<C_0082dd78*>(owner->field_004c);
                        void* globalState =
                            reinterpret_cast<void*>(GetGlobalState_50D190());
                        void* characterRegistry = reinterpret_cast<void*>(
                            CSF_AT(uint32_t, globalState, 0xE8));
                        const uint32_t characterId = character->field_0040;
                        void* characterInfo =
                            reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
                                CSF_AT(void**, characterRegistry, 0)[0x08 / 4])(
                                    characterRegistry, nullptr, characterId);
                        CSF_AT(int, requestBytes, 0x08) =
                            CSF_AT(int, characterInfo, 0x2C);
                    }
                }
                else
                {
                    sounds = reinterpret_cast<void*>(
                        CSF_AT(uint32_t, soundEntry, 0x20));
                    CSF_AT(int, requestBytes, 0x08) =
                        CSF_AT(int, sounds, 8 * timerIndex);
                }

                auto* listenerOwner =
                    reinterpret_cast<C_00833908*>(self->field_0008);
                CSF_AT(int, requestBytes, 0x10) = -1082130432;
                const uint32_t listenerValue1 = listenerOwner->field_0020;
                CSF_AT(uint32_t, requestBytes, 0x28) = listenerValue1;
                const uint32_t listenerValue2 = listenerOwner->field_0024;
                CSF_AT(uint32_t, requestBytes, 0x2C) = listenerValue2;
                CSF_AT(uint32_t, listenerBytes, 0x08) = listenerValue2;
                CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D448);
                CSF_AT(uint32_t, listenerBytes, 0x04) = listenerValue1;
                CSF_AT(int, listenerBytes, 0x0C) = 1232348208;

                auto* soundManager =
                    reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
                S_00521df0_SubmitSoundRequest(
                    soundManager, nullptr, &request,
                    reinterpret_cast<int>(listenerBytes));

                timerCount = CSF_AT(int32_t, listenerBytes, 0x14);
                CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, requestBytes, 0x24) = CSF_VTBL(0x0082D410);
            }
        }
    }
}

HOOK(0x004A38E0, C_008344f0_UpdateAnimationSounds, "auto");
