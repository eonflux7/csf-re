#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, WeaponFirearms_InitializeState, 0x00475A10, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008344f0_CommitAnimation, 0x004A3AE0, (C_008344f0* self, void* edx, std::uint32_t a2, std::uint32_t a3, std::uint32_t a4));
CSF_FUNC(int, __fastcall, C_008346e8_GetAnimationSlot, 0x004A6760, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_ResolveAnimation, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

static int __fastcall C_008346e8_StartAnimation(
    C_008346e8* self, void* /*edx*/, int channel, int animationId,
    float duration, int resolvedId, int /*a6*/)
{
    if (animationId == -1)
        return -1;

    alignas(8) std::uint8_t stateBytes[0x28]{};

    void* setup = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC4);
    CSF_AT(int, stateBytes, 0x24) =
        SceneMissionSetup_GetAnimationEntry(setup, nullptr, animationId);

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
    void* accessorState =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    int animation = SceneMissionSetup_ResolveAnimation(
        setup, nullptr, animationId, reinterpret_cast<unsigned int*>(&resolvedId));
    void* slot = reinterpret_cast<void*>(
        AccessorState_ResolveSlot(accessorState, nullptr, animation));
    if (!slot)
        return -1;

    void* controller = reinterpret_cast<void*>(
        reinterpret_cast<C_00833908*>(self->field_0008)->vGetField016c());
    void* flags =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr);

    if ((CSF_AT(std::uint8_t, flags, 0) & 1) == 0)
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return -1;
    }

    void* animationController =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x68 / 4])(controller, nullptr);
    CSF_AT(int, stateBytes, 0x20) =
        reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, animationController, 0)[0x18 / 4])(
                animationController, nullptr, slot);

    if (CSF_AT(int, stateBytes, 0x20) !=
        C_008346e8_GetAnimationSlot(self, nullptr, channel))
    {
        C_00833908_GetTargetName(
            reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return -1;
    }

    WeaponFirearms_InitializeState(
        reinterpret_cast<std::uint32_t*>(stateBytes), nullptr);

    void* entry = reinterpret_cast<void*>(CSF_AT(int, stateBytes, 0x24));
    CSF_AT(std::uint8_t, stateBytes, 0x0C) = self->field_0081
        ? self->field_0080
        : CSF_AT(std::uint8_t, entry, 0x58);

    float blendTime =
        CSF_AT(float, entry, 0x34) * g_SceneLoadMillisecondsToSeconds;
    std::uint8_t entryFlag1 = CSF_AT(std::uint8_t, entry, 0x59);
    std::uint8_t entryFlag2 = CSF_AT(std::uint8_t, entry, 0x5A);
    std::uint8_t skipBlend = self->field_0078;

    CSF_AT(std::uint8_t, stateBytes, 0x0D) = entryFlag1;
    CSF_AT(float, stateBytes, 0x04) = blendTime;
    CSF_AT(std::uint8_t, stateBytes, 0x0E) = entryFlag2;
    CSF_AT(std::uint32_t, stateBytes, 0) = self->field_007c;

    if (skipBlend)
    {
        CSF_AT(float, stateBytes, 0x04) = 0.0f;
        self->field_0078 = 0;
    }

    bool started =
        reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*, float*)>(
            CSF_AT(void**, animationController, 0)[0x14 / 4])(
                animationController, nullptr, CSF_AT(int, stateBytes, 0x20),
                slot, reinterpret_cast<float*>(stateBytes));
    if (!started)
        return -1;

    int mode = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        self->vtbl[0x80 / 4])(self, nullptr, channel);
    if (mode == 0)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, animationController, 0)[0x1C / 4])(
                animationController, nullptr, 20, 0);
    }

    if (mode != 1)
    {
        if (duration > g_fZero)
        {
            self->field_0064 = duration - CSF_AT(float, stateBytes, 0);
        }
        else if (CSF_AT(std::uint8_t, stateBytes, 0x0C))
        {
            self->field_0064 = 3.4028235e38f;
        }
        else
        {
            double value = C_Unknown_GetValue(slot, nullptr);
            self->field_0064 =
                static_cast<float>(value - CSF_AT(float, stateBytes, 0));
        }
    }

    C_008344f0_CommitAnimation(self, nullptr, channel, animationId, resolvedId);
    int result = resolvedId;
    self->field_007c = 0;
    self->field_0081 = 0;
    return result;
}

HOOK(0x004A69A0, C_008346e8_StartAnimation, "auto");
