#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, WeaponFirearms_InitializeState, 0x00475A10, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, GetActionPlaybackFlag, 0x004920D0, (int a1));
CSF_FUNC(int, __fastcall, C_008344f0_RecordAnimation, 0x004A3AE0, (C_008344f0* self, void* edx, std::uint32_t a2, std::uint32_t a3, std::uint32_t a4));
CSF_FUNC(char, __stdcall, GetAnimationPlaybackFlag, 0x004A4380, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, ResolveAnimationIndex, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, GetRequestedActionName, 0x00761380, (int a1));
CSF_FUNC(char*, __cdecl, GetCurrentActionName, 0x00761390, (int a1));
CSF_FUNC(char*, __cdecl, GetActionStateName, 0x007613A0, (int a1));
CSF_FUNC(char*, __cdecl, GetActionDetailName, 0x007613B0, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static int __fastcall C_00834598_PlayAnimation(
    C_00834598* self, void* /*edx*/, int action,
    std::uint32_t* animation, float duration, int animationIndex, float startTime)
{
    alignas(8) std::uint8_t playbackBytes[0x28];

    if (animation == reinterpret_cast<std::uint32_t*>(-1))
    {
        auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
        void* actor = reinterpret_cast<void*>(owner->vGetField016c());
        void* controller =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, actor, 0)[0x68 / 4])(actor, nullptr);

        int actionMode =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                self->vtbl[0x80 / 4])(self, nullptr, action);
        if (!actionMode || actionMode == 2)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, controller, 0)[0x1C / 4])(
                    controller, nullptr, 0, 0);
        }
        else
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, controller, 0)[0x1C / 4])(
                    controller, nullptr, 20, 0);
        }

        owner = reinterpret_cast<C_00833908*>(self->field_0008);
        C_00833908_GetTargetName(owner, nullptr);
        GetRequestedActionName(action);

        void* related = reinterpret_cast<void*>(
            reinterpret_cast<C_00833908*>(self->field_0008)->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, related, 0)[0xFC / 4])(related, nullptr);

        int state = CSF_AT(int, owner, 0x1D8);
        GetActionStateName(state);
        int currentAction = CSF_AT(int, owner, 0x1C4);
        GetCurrentActionName(currentAction);
        GetActionDetailName(CSF_AT(int, owner, 0x1E0), currentAction, state);
        return -1;
    }

    void* sceneState = reinterpret_cast<void*>(GetValue8B0554());
    void* animationManager = CSF_AT(void*, sceneState, 0x1FC4);
    void* entry = reinterpret_cast<void*>(SceneMissionSetup_GetAnimationEntry(
        animationManager, nullptr, reinterpret_cast<int>(animation)));

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    CSF_AT(int, playbackBytes, 0x24) =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    int resolvedIndex = ResolveAnimationIndex(
        animationManager, nullptr, reinterpret_cast<int>(animation),
        reinterpret_cast<unsigned int*>(&animationIndex));
    if (resolvedIndex == -1)
        return -1;

    int slot = AccessorState_ResolveSlot(
        reinterpret_cast<void*>(CSF_AT(int, playbackBytes, 0x24)),
        nullptr, resolvedIndex);
    CSF_AT(int, playbackBytes, 0x24) = slot;
    if (!slot)
        return -1;

    if (CSF_AT(std::uint8_t, entry, 0x58) &&
        reinterpret_cast<std::uint32_t>(animation) == self->field_0048 &&
        !static_cast<std::uint8_t>(self->vSlot34()))
    {
        if (duration > g_fZero)
            self->field_0064 = duration;
        else if (CSF_AT(std::uint8_t, entry, 0x58))
            self->field_0064 = g_FloatMax;
        else
            self->field_0064 = static_cast<float>(
                C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr));
        return animationIndex;
    }

    void* actor = reinterpret_cast<void*>(
        reinterpret_cast<C_00833908*>(self->field_0008)->vGetField016c());
    auto* flags =
        reinterpret_cast<std::uint8_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actor, 0)[0x20 / 4])(actor, nullptr);
    if ((*flags & 1) == 0)
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return -1;
    }

    void* controller =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actor, 0)[0x68 / 4])(actor, nullptr);
    if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x58 / 4])(controller, nullptr))
        return -1;

    int availability =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr, CSF_AT(int, playbackBytes, 0x24));
    if (availability == -1)
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return animationIndex;
    }
    if (availability)
        return -1;

    CSF_AT(int, playbackBytes, 0x20) =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            self->vtbl[0x80 / 4])(self, nullptr, action);
    WeaponFirearms_InitializeState(
        reinterpret_cast<std::uint32_t*>(playbackBytes), nullptr);

    CSF_AT(std::uint8_t, playbackBytes, 0x0C) =
        CSF_AT(std::uint8_t, entry, 0x58);
    CSF_AT(std::uint8_t, playbackBytes, 0x0D) =
        CSF_AT(std::uint8_t, entry, 0x59);
    CSF_AT(std::uint8_t, playbackBytes, 0x0E) =
        CSF_AT(std::uint8_t, entry, 0x5A);
    CSF_AT(std::uint8_t, playbackBytes, 0x1C) =
        static_cast<std::uint8_t>(GetActionPlaybackFlag(action));
    CSF_AT(char, playbackBytes, 0x1D) =
        GetAnimationPlaybackFlag(action, reinterpret_cast<int>(entry));

    double value = C_Unknown_GetValue(
        reinterpret_cast<void*>(CSF_AT(int, playbackBytes, 0x24)), nullptr);
    if (value < startTime)
    {
        CSF_AT(float, playbackBytes, 0) = static_cast<float>(
            C_Unknown_GetValue(
                reinterpret_cast<void*>(CSF_AT(int, playbackBytes, 0x24)),
                nullptr));
    }
    else
        CSF_AT(float, playbackBytes, 0) = startTime;

    CSF_AT(float, playbackBytes, 0x04) =
        CSF_AT(float, entry, 0x34) * g_SceneLoadMillisecondsToSeconds;
    if (self->field_0078)
    {
        self->field_0078 = 0;
        CSF_AT(float, playbackBytes, 0x04) = g_fZero;
    }

    int playbackSlot = CSF_AT(int, playbackBytes, 0x24);
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*, int, int, float*)>(
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, 0, playbackSlot,
                reinterpret_cast<float*>(playbackBytes)))
        return -1;

    if (!CSF_AT(int, playbackBytes, 0x20))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, controller, 0)[0x1C / 4])(
                controller, nullptr, 20, 0);
    }

    C_008344f0_RecordAnimation(
        self, nullptr, static_cast<std::uint32_t>(action),
        reinterpret_cast<std::uint32_t>(animation),
        static_cast<std::uint32_t>(animationIndex));

    if (duration > g_fZero)
        self->field_0064 = duration - CSF_AT(float, playbackBytes, 0);
    else if (CSF_AT(std::uint8_t, playbackBytes, 0x0C))
        self->field_0064 = g_FloatMax;
    else
        self->field_0064 = static_cast<float>(
            C_Unknown_GetValue(reinterpret_cast<void*>(playbackSlot), nullptr) -
            CSF_AT(float, playbackBytes, 0));

    return animationIndex;
}

HOOK(0x004A4F50, C_00834598_PlayAnimation, "auto");
