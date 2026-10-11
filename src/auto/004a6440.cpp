#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008344f0_SetAnimationState, 0x004A3AE0,
         (C_008344f0* self, void* edx, uint32_t owner, uint32_t animation, uint32_t variant));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_ResolveAnimationVariant, 0x00519B50,
         (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

static uint32_t __fastcall C_00834640_StartAnimation(
    C_00834640* self, void* /*edx*/, uint32_t owner,
    uint32_t animation, float duration, uint32_t variant, float startTime)
{
    alignas(8) uint8_t playbackParameters[0x24];
    CSF_AT(C_00834640*, playbackParameters, 0x20) = self;

    if (animation == uint32_t(-1))
        return uint32_t(-1);

    void* sceneState = reinterpret_cast<void*>(GetValue8B0554());
    void* missionSetup = CSF_AT(void*, sceneState, 0x1FC4);
    void* animationEntry = reinterpret_cast<void*>(
        SceneMissionSetup_GetAnimationEntry(missionSetup, nullptr, static_cast<int>(animation)));

    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalState, nullptr));
    void* accessorState =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    int resolvedVariant = SceneMissionSetup_ResolveAnimationVariant(
        missionSetup, nullptr, static_cast<int>(animation), &variant);
    void* animationSlot = reinterpret_cast<void*>(
        AccessorState_ResolveSlot(accessorState, nullptr, resolvedVariant));
    if (!animationSlot)
        return uint32_t(-1);

    void* animationController = reinterpret_cast<void*>(
        reinterpret_cast<C_00833908*>(self->field_0008)->vGetField016c());
    void* controllerFlags =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, animationController, 0)[0x20 / 4])(animationController, nullptr);
    if ((CSF_AT(uint8_t, controllerFlags, 0) & 1) == 0)
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return uint32_t(-1);
    }

    void* player =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, animationController, 0)[0x68 / 4])(animationController, nullptr);

    uint8_t looping = CSF_AT(uint8_t, animationEntry, 0x58);
    CSF_AT(float, playbackParameters, 0x00) = 0.0f;
    CSF_AT(float, playbackParameters, 0x04) = 0.0f;
    CSF_AT(float, playbackParameters, 0x08) = 0.0f;
    CSF_AT(float, playbackParameters, 0x10) = 1.0f;
    CSF_AT(float, playbackParameters, 0x14) = 1.0f;
    CSF_AT(float, playbackParameters, 0x18) = 1.0f;
    CSF_AT(uint8_t, playbackParameters, 0x1C) = 0;
    CSF_AT(uint8_t, playbackParameters, 0x1D) = 1;
    CSF_AT(uint8_t, playbackParameters, 0x0C) = looping;
    CSF_AT(uint8_t, playbackParameters, 0x0D) = 0;
    CSF_AT(uint8_t, playbackParameters, 0x0E) = 0;

    if (C_Unknown_GetValue(animationSlot, nullptr) < startTime)
        CSF_AT(float, playbackParameters, 0x00) =
            static_cast<float>(C_Unknown_GetValue(animationSlot, nullptr));
    else
        CSF_AT(float, playbackParameters, 0x00) = startTime;

    CSF_AT(float, playbackParameters, 0x04) =
        CSF_AT(float, animationEntry, 0x34) * g_SceneLoadMillisecondsToSeconds;
    if (self->field_0078)
    {
        CSF_AT(float, playbackParameters, 0x04) = 0.0f;
        self->field_0078 = 0;
    }

    if (!reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*, void*)>(
            CSF_AT(void**, player, 0)[0x14 / 4])(
                player, nullptr, 0, animationSlot, playbackParameters))
        return uint32_t(-1);

    C_008344f0_SetAnimationState(self, nullptr, owner, animation, variant);

    if (duration > g_fZero)
        self->field_0064 = duration - CSF_AT(float, playbackParameters, 0x00);
    else if (CSF_AT(uint8_t, playbackParameters, 0x0C))
        self->field_0064 = FLT_MAX;
    else
        self->field_0064 = static_cast<float>(
            C_Unknown_GetValue(animationSlot, nullptr) -
            CSF_AT(float, playbackParameters, 0x00));

    return variant;
}

HOOK(0x004A6440, C_00834640_StartAnimation, "auto");
