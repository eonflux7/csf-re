#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833b88_IsInactive, 0x0048E100, (C_00833b88* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(char, __fastcall, CMenuMpPause_UpdateFriends, 0x00534B90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, MpSpawn_QueryFlagForGlobal, 0x00551650, (void* self, void*));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint8_t, g_FriendsMode, 0x008B067C);

static int __fastcall CMenuMpPause_Update(
    CMenuMpPause* self, void* /*edx*/, int deltaBits)
{
    self->field_0040 = static_cast<uint32_t>(deltaBits);
    auto* timeState =
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    if (!self->field_0014)
    {
        auto* globalState =
            reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
        const uintptr_t schedulerState = reinterpret_cast<uintptr_t>(
            C_00861120_GetSchedulerProxy(globalState, nullptr));

        if (schedulerState == 1)
        {
            void* panel = reinterpret_cast<void*>(self->field_0038);
            self->field_0014 = 0xFFFFFFFF;
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, 1);
            self->vInitPauseMenuPanels();
        }
        else if (schedulerState == 2)
        {
            self->field_0014 = 0xFFFFFFFF;
        }
    }

    if (g_FriendsMode)
        CMenuMpPause_UpdateFriends(self, nullptr);

    auto* sceneContext = reinterpret_cast<C_00833b88*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));

    void* control = reinterpret_cast<void*>(self->field_0024);
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr))
    {
        if (!sceneContext || CSF_AT(uint8_t, sceneContext, 0x3A4))
        {
            control = reinterpret_cast<void*>(self->field_0024);
            ((int(__fastcall*)(void*, void*, int, int, int))
                CSF_AT(void**, control, 0)[0x68 / 4])(
                    control, nullptr, 0, 1, 1);
        }
    }
    else if (sceneContext && !CSF_AT(uint8_t, sceneContext, 0x3A4))
    {
        control = reinterpret_cast<void*>(self->field_0024);
        ((int(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, control, 0)[0x68 / 4])(
                control, nullptr, 1, 1, 1);
    }

    control = reinterpret_cast<void*>(self->field_001c);
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr))
    {
        int result = MpSpawn_QueryFlagForGlobal(
            reinterpret_cast<void*>(GetGlobal8B0688()), nullptr);
        if (result)
        {
            if (sceneContext)
            {
                result = C_00833b88_IsInactive(sceneContext, nullptr);
                if (!static_cast<uint8_t>(result))
                    return result;
            }

            control = reinterpret_cast<void*>(self->field_001c);
            return ((int(__fastcall*)(void*, void*, int, int, int))
                CSF_AT(void**, control, 0)[0x68 / 4])(
                    control, nullptr, 0, 1, 1);
        }
        return result;
    }

    int result = MpSpawn_QueryFlagForGlobal(
        reinterpret_cast<void*>(GetGlobal8B0688()), nullptr);
    if (result)
    {
        if (!sceneContext)
            return result;

        result = C_00833b88_IsInactive(sceneContext, nullptr);
        if (static_cast<uint8_t>(result))
            return result;
    }

    control = reinterpret_cast<void*>(self->field_001c);
    return ((int(__fastcall*)(void*, void*, int, int, int))
        CSF_AT(void**, control, 0)[0x68 / 4])(
            control, nullptr, 1, 1, 1);
}

HOOK(0x005351D0, CMenuMpPause_Update, "auto");
