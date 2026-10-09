#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareGlobalState, 0x0050FF30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, SceneLoader_UpdateLoadingState, 0x00521F40, (int a1, int a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SetActiveContext, 0x00697360, (int a1));
CSF_FUNC(int, __fastcall, ResetFourDwords, 0x0069AEB0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_BeginContext, 0x0069AEC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_EndContext, 0x0069AEF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_ActiveSceneContext, 0x008BAF90);

static int __fastcall S_004fe580_FinalizePendingScene(
    S_004fe580* self, void* /*edx*/)
{
    const int globalState = GetGlobalState_50D190();
    void* sceneState = CSF_AT(void*, reinterpret_cast<void*>(globalState), 0x5C);
    if (CSF_AT(int, sceneState, 0x28) != 3)
        return globalState;

    SceneLoader_PrepareGlobalState(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);

    void* context = static_cast<std::uint8_t*>(sceneState) + 0x10;
    ResetFourDwords(context, nullptr);
    CSF_AT(std::uint32_t, sceneState, 0x1C) =
        CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x2260);
    SceneLoader_BeginContext(context, nullptr);
    SceneLoader_SetActiveContext(reinterpret_cast<int>(context));
    g_ActiveSceneContext = context;

    void* pendingScene = CSF_AT(void*, self, 0x100);
    const std::uint32_t sceneArgument =
        CSF_AT(std::uint32_t, pendingScene, 0x38);
    reinterpret_cast<void (__fastcall*)(S_004fe580*, void*, std::uint32_t, int)>(
        reinterpret_cast<void**>(self->field_0000)[0x30 / 4])(
            self, nullptr, sceneArgument, 0);

    GetGlobalValue_521BB0();
    SceneLoader_UpdateLoadingState(0, 1);
    GlobalState_ResetPending(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    CSF_AT(std::uint32_t, self, 0x100) = 0;
    SceneLoader_EndContext(context, nullptr);
    const int result = SceneLoader_SetActiveContext(0);
    g_ActiveSceneContext = nullptr;
    self->field_0028 = 0;
    Nullsub_506AC0();
    return result;
}

HOOK(0x004FE730, S_004fe580_FinalizePendingScene, "auto");
