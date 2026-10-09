#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareCompletion, 0x0050FF30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, SceneLoader_SetCompletionOptions, 0x00521F40, (int a1, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneResources, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SetActiveLoadContext, 0x00697360, (int a1));
CSF_FUNC(int, __fastcall, ResetFourDwords, 0x0069AEB0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_ActivateLoadContext, 0x0069AEC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_DeactivateLoadContext, 0x0069AEF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneResources, 0x006CADD0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_ActiveSceneLoadContext, 0x008BAF90);

static int __fastcall S_004fe580_CompleteSceneLoad(
    S_004fe580* self, void* /*edx*/)
{
    const int initialState = GetGlobalState_50D190();
    auto* loadState = CSF_AT(uint8_t*, reinterpret_cast<void*>(initialState), 0x5C);

    if (CSF_AT(uint32_t, loadState, 0x28) != 3)
        return initialState;

    SceneLoader_PrepareCompletion(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);

    void* loadContext = loadState + 0x10;
    ResetFourDwords(loadContext, nullptr);
    CSF_AT(uint32_t, loadState, 0x1C) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x2260);
    SceneLoader_ActivateLoadContext(loadContext, nullptr);
    SceneLoader_SetActiveLoadContext(reinterpret_cast<int>(loadContext));
    g_ActiveSceneLoadContext = loadContext;

    reinterpret_cast<void (__fastcall*)(S_004fe580*, void*)>(
        reinterpret_cast<void**>(self->field_0000)[0x2C / 4])(self, nullptr);
    self->field_0020 = 0;

    const int resources = SceneLoader_GetSceneResources(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    SceneLoader_FinalizeSceneResources(reinterpret_cast<void*>(resources), nullptr);

    const int sectorIndex =
        CSF_AT(int, CSF_AT(void*, self, 0x100), 0x38);
    auto* sectors = reinterpret_cast<uint8_t*>(self->field_0030);
    if (CSF_AT(uint8_t, sectors,
               static_cast<uint32_t>(sectorIndex) * 0x58 + 0x1C) != 0)
    {
        reinterpret_cast<void (__fastcall*)(S_004fe580*, void*, int)>(
            reinterpret_cast<void**>(self->field_0000)[0x38 / 4])(
                self, nullptr, sectorIndex);
    }

    GetGlobalValue_521BB0();
    SceneLoader_SetCompletionOptions(0, 1);
    GlobalState_ResetPending(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    CSF_AT(uint32_t, self, 0x100) = 0;
    SceneLoader_DeactivateLoadContext(loadContext, nullptr);
    const int result = SceneLoader_SetActiveLoadContext(0);
    g_ActiveSceneLoadContext = nullptr;
    self->field_0024 = 0;
    self->field_0028 = 0;
    Nullsub_506AC0();
    return result;
}

HOOK(0x004FF840, S_004fe580_CompleteSceneLoad, "auto");
