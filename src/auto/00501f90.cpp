#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetBaseOffset8, 0x004448C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_ApplySceneAnimations, 0x004E9370, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_PrepareSceneResources, 0x004E9AC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeGlobalResources, 0x004FBED0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_PrepareScene, 0x004FE130, ());
CSF_FUNC(int, __cdecl, SceneLoader_FinalizeSceneResources, 0x004FE430, ());
CSF_FUNC(int, __cdecl, SceneLoader_InitializeSceneObjects, 0x004FEBA0, ());
CSF_FUNC(int, __cdecl, SceneLoader_LoadSceneResources, 0x004FED20, ());
CSF_FUNC(void*, __cdecl, SceneLoader_LoadSceneGeometry, 0x004FEDB0, ());
CSF_FUNC(int, __stdcall, SceneLoader_ProcessObjectCollection, 0x004FF2F0, (int a1, char a2));
CSF_FUNC(int, __fastcall, SceneLoader_ProcessSceneEntries, 0x004FFB70, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_ProcessWorld, 0x00501360, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetModelManager, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeObjectProvider, 0x006D4450, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void SceneLoader_AdvanceLoadingProgress(CSceneLoaderBase* self)
{
    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C))
    {
        void* progress = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, progress, 0)[0x44 / 4])(progress, nullptr, 10.0f);
    }
}

static int __fastcall CSceneLoaderBase_FinalizeSceneLoading(
    CSceneLoaderBase* self, void* /*edx*/)
{
    // Preserve the ECX receiver shown by the assembly, while retaining the
    // supplied bindings for these addresses.
    reinterpret_cast<int (__fastcall*)(void*, void*)>(
        SceneLoader_PrepareScene)(self, nullptr);
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        SceneLoader_LoadSceneGeometry)(self, nullptr);
    SceneLoader_AdvanceLoadingProgress(self);

    reinterpret_cast<int (__fastcall*)(void*, void*)>(
        SceneLoader_LoadSceneResources)(self, nullptr);
    SceneLoader_AdvanceLoadingProgress(self);

    SceneLoader_FinalizeGlobalResources(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
    SceneLoader_AdvanceLoadingProgress(self);

    SceneLoader_ProcessSceneEntries(
        self, nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(
                CSF_AT(void*, g_pCurrentScene, 0xBC)) + 8));

    int entries = GetBaseOffset8(
        CSF_AT(void*, g_pCurrentScene, 0xC0), nullptr);
    SceneLoader_ProcessSceneEntries(self, nullptr, entries);
    SceneLoader_AdvanceLoadingProgress(self);

    reinterpret_cast<int (__fastcall*)(void*, void*)>(
        SceneLoader_InitializeSceneObjects)(self, nullptr);
    SceneLoader_AdvanceLoadingProgress(self);

    SceneLoader_PrepareSceneResources(
        reinterpret_cast<void*>(GetValue8B0554()), nullptr);
    reinterpret_cast<int (__fastcall*)(void*, void*)>(
        SceneLoader_FinalizeSceneResources)(self, nullptr);

    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* animations =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, animations, 0)[0x14 / 4])(animations, nullptr);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x04))
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, animations, 0)[0x24 / 4])(
                animations, nullptr, CSF_STR(0x0083F55C, "LogAnims.and"));

        void* models = reinterpret_cast<void*>(SceneLoader_GetModelManager(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, models, 0)[0x1C / 4])(
                models, nullptr, CSF_STR(0x0083F54C, "LogModels.m3d"));
    }

    SceneLoader_ApplySceneAnimations(
        reinterpret_cast<void*>(GetValue8B0554()), nullptr);

    void* models = reinterpret_cast<void*>(SceneLoader_GetModelManager(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    int world = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, models, 0)[0x44 / 4])(models, nullptr);
    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        SceneLoader_ProcessWorld)(self, nullptr, world);

    models = reinterpret_cast<void*>(SceneLoader_GetModelManager(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* worldObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, models, 0)[0x44 / 4])(models, nullptr);
    int collection = SceneLoader_GetWorldObjectCollection(worldObject, nullptr);
    reinterpret_cast<int (__fastcall*)(void*, void*, int, char)>(
        SceneLoader_ProcessObjectCollection)(self, nullptr, collection, 0);

    models = reinterpret_cast<void*>(SceneLoader_GetModelManager(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* reference =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, models, 0)[0x40 / 4])(models, nullptr);
    void* referencedObject = GetReferencedObject(reference, nullptr);
    reinterpret_cast<int (__fastcall*)(void*, void*, int, char)>(
        SceneLoader_ProcessObjectCollection)(
            self, nullptr, reinterpret_cast<int>(referencedObject), 1);

    accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);

    // The packet labels the final direct-jump destination as a vtable.
    (void)CSF_VTBL(0x006D4450);
    return SceneLoader_FinalizeObjectProvider(provider, nullptr);
}

HOOK(0x00501F90, CSceneLoaderBase_FinalizeSceneLoading, "auto");
