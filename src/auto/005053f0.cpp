#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __stdcall, SceneLoader_SerializeSceneSettings, 0x0043FAF0, (void* a1, int a2));
CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, GetBaseOffset8, 0x004448C0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_SelectMainScene, 0x0044F650, ());
CSF_FUNC(int, __cdecl, SceneLoader_GetScene, 0x00450610, (uint32_t a1));
CSF_FUNC(int, __cdecl, SceneLoader_SelectScene, 0x00450630, (int a1, int a2));
CSF_FUNC(void*, __cdecl, SceneLoader_CreateScene, 0x004565D0, ());
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_RegisterResource, 0x004E8E00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_ApplySceneObjectSettings, 0x004E9370, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_PrepareResources, 0x004E9AC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneSubsystem, 0x004FBED0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeSceneSubsystem, 0x004FCB90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_PrepareWorld, 0x004FE130, ());
CSF_FUNC(int, __cdecl, SceneLoader_FinalizeWorld, 0x004FEBA0, ());
CSF_FUNC(int, __cdecl, SceneLoader_LoadWorldResources, 0x004FED20, ());
CSF_FUNC(void*, __cdecl, SceneLoader_PrepareSingleScene, 0x004FEDB0, ());
CSF_FUNC(unsigned int, __cdecl, SceneLoader_FinalizeWorldObjects, 0x004FF160, ());
CSF_FUNC(int, __fastcall, SceneLoader_ProcessSceneObjects, 0x004FFB70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_SerializeAdditionalSettings, 0x00500990, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_PrepareWorldMaterials, 0x00501360, (int a1));
CSF_FUNC(int, __stdcall, SceneLoader_SerializeSceneEntries, 0x00505210, (void* a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0,
         (int a1, int (__cdecl *a2)(uint32_t*, int), int a3));
CSF_FUNC(int, __cdecl, RpWorldForAllMaterials, 0x0058CF30,
         (int a1, int (__cdecl *a2)(uint32_t, int), int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneService, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderWorld, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* const g_SceneLoaderResourceTable = CSF_VTBL(0x004E8E00);

static void* SceneLoader_Pointer(int address)
{
    return reinterpret_cast<void*>(address);
}

static bool SceneLoader_BeginArchiveField(void* archive, bool objectReference = false)
{
    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = objectReference ? 1 : 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static void SceneLoader_SerializeInteger(void* archive, void* value)
{
    if (SceneLoader_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            ((int (__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, backend, 0)[72 / 4])(backend, nullptr, value, archive) == -1;
    }
}

static void SceneLoader_SerializeByte(void* archive, void* value)
{
    if (SceneLoader_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            ((int (__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, backend, 0)[68 / 4])(backend, nullptr, value, archive) == -1;
    }
}

static void* SceneLoader_ResolveSceneService()
{
    return SceneLoader_Pointer(
        SceneLoader_GetSceneService(SceneLoader_Pointer(GetGlobalInt()), nullptr));
}

static bool SceneLoader_HasSceneEntry(C_0082d410* self, uint32_t index)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x30);
    return begin != 0 &&
           index < (CSF_AT(uint32_t, self, 0x34) - begin) / 88;
}

static bool SceneLoader_IsSceneEntryActive(C_0082d410* self, int entryOffset)
{
    return CSF_AT(uint8_t, CSF_AT(uint8_t*, self, 0x30), entryOffset + 29) != 0;
}

static void SceneLoader_SerializeSceneObject(int scene, void* archive)
{
    void* object = reinterpret_cast<uint8_t*>(SceneLoader_Pointer(scene)) + 4;
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, object, 0)[8 / 4])(object, nullptr, archive);
}

static void SceneLoader_SerializeMainScene(void* archive)
{
    SceneLoader_SelectMainScene();
    void* object = reinterpret_cast<uint8_t*>(g_pCurrentScene) + 4;
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, object, 0)[8 / 4])(object, nullptr, archive);
}

static void SceneLoader_UpdateLoadingProgress(C_0082d410* self)
{
    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, SceneLoader_Pointer(GetGlobalState_50D190()), 0x1C))
    {
        void* object =
            CSF_AT(void*, SceneLoader_Pointer(GetGlobalState_50D190()), 0x1C);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[68 / 4])(object, nullptr, 1092616192);
    }
}

static void SceneLoader_NotifySceneService()
{
    void* service = SceneLoader_ResolveSceneService();
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, service, 0)[44 / 4])(service, nullptr);
}

static void SceneLoader_FillMaterialContext(uint32_t* context)
{
    void* lookup = LookupObjectById(SceneLoader_Pointer(GetGlobalInt()), nullptr);
    void* settings =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, lookup, 0)[24 / 4])(lookup, nullptr);
    context[1] = CSF_AT(uint32_t, settings, 32);

    lookup = LookupObjectById(SceneLoader_Pointer(GetGlobalInt()), nullptr);
    settings =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, lookup, 0)[24 / 4])(lookup, nullptr);
    context[2] = CSF_AT(uint32_t, settings, 28);
}

// Use the declared base type; CSceneLoaderBase is a descriptive packet name.
static int __fastcall CSceneLoaderBase_SerializeScene(
    C_0082d410* self, void* /*edx*/, void* archive)
{
    uint8_t* object = reinterpret_cast<uint8_t*>(self);
    uint32_t materialContext[0x18 / sizeof(uint32_t)] = {};

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 4);
        failed =
            !((bool (__fastcall*)(void*, void*, int, void*))
                CSF_AT(void**, backend, 0)[128 / 4])(backend, nullptr, 0, archive);
    }
    materialContext[3] = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int (__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, backend, 0)[72 / 4])(
                    backend, nullptr, object + 12, archive) == -1;
    }

    if (SceneLoader_BeginArchiveField(archive))
        SceneLoader_SerializeSceneEntries(object + 44, reinterpret_cast<int>(archive));

    if (SceneLoader_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            ((int (__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, backend, 0)[56 / 4])(
                    backend, nullptr, object + 76, archive) == -1;
    }

    SceneLoader_SerializeByte(archive, object + 88);

    if (SceneLoader_BeginArchiveField(archive))
        SceneLoader_SerializeSceneSettings(object + 124, reinterpret_cast<int>(archive));

    SceneLoader_SerializeByte(archive, object + 180);

    if (SceneLoader_BeginArchiveField(archive))
        SceneLoader_SerializeAdditionalSettings(
            object + 240, nullptr, reinterpret_cast<int>(archive));

    if (SceneLoader_BeginArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            ((int (__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, object + 32, archive) == -1;
    }

    SceneLoader_SerializeInteger(archive, object + 20);
    SceneLoader_SerializeInteger(archive, object + 24);

    if (SceneLoader_BeginArchiveField(archive, true))
        Archive_SerializeObjectRef(object + 232, reinterpret_cast<bool*>(archive));

    if (SceneLoader_BeginArchiveField(archive, true))
        SceneLoader_SerializeReference(object + 236, reinterpret_cast<bool*>(archive));

    uint8_t mainScenePresent = g_SceneLoaderMainScenePresent;
    SceneLoader_SerializeByte(archive, &mainScenePresent);

    void* service = SceneLoader_ResolveSceneService();
    CSF_AT(int, self, 0x108) =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, service, 0)[152 / 4])(service, nullptr);
    SceneLoader_SerializeInteger(archive, object + 264);

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        if (CSF_AT(uint8_t, archive, 0) == 0)
        {
            uint32_t sceneIndex = 0;
            int entryOffset = 0;
            while (SceneLoader_HasSceneEntry(self, sceneIndex))
            {
                if (SceneLoader_IsSceneEntryActive(self, entryOffset))
                {
                    const int scene = SceneLoader_GetScene(sceneIndex);
                    SceneLoader_SelectScene(sceneIndex, 0);
                    SceneLoader_SerializeSceneObject(scene, archive);
                }
                ++sceneIndex;
                entryOffset += 88;
            }
            if (mainScenePresent)
                SceneLoader_SerializeMainScene(archive);
        }
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0)
    {
        if (CSF_AT(uint8_t, archive, 0) == 0)
        {
            uint32_t sceneIndex = 0;
            int entryOffset = 0;
            while (SceneLoader_HasSceneEntry(self, sceneIndex))
            {
                if (SceneLoader_IsSceneEntryActive(self, entryOffset))
                {
                    if (!SceneLoader_GetScene(sceneIndex))
                    {
                        SceneLoader_CreateScene();
                        SceneLoader_GetScene(sceneIndex);
                    }
                    SceneLoader_SelectScene(sceneIndex, 0);
                }
                ++sceneIndex;
                entryOffset += 88;
            }

            if (CSF_AT(uint8_t, self, 0xE0))
            {
                SceneLoader_NotifySceneService();
            }
            else
            {
                SceneLoader_PrepareWorld();

                const uint32_t begin = CSF_AT(uint32_t, self, 0x30);
                if (begin && (CSF_AT(uint32_t, self, 0x34) - begin) / 88 == 1)
                    SceneLoader_PrepareSingleScene();

                SceneLoader_UpdateLoadingProgress(self);
                SceneLoader_LoadWorldResources();
                SceneLoader_UpdateLoadingProgress(self);
                SceneLoader_PrepareSceneSubsystem(
                    SceneLoader_Pointer(GetGlobalValue_4FBDE0()), nullptr);
                SceneLoader_UpdateLoadingProgress(self);

                sceneIndex = 0;
                entryOffset = 0;
                while (SceneLoader_HasSceneEntry(self, sceneIndex))
                {
                    if (SceneLoader_IsSceneEntryActive(self, entryOffset))
                    {
                        SceneLoader_SelectScene(sceneIndex, 0);
                        SceneLoader_ProcessSceneObjects(
                            self, nullptr, CSF_AT(int, g_pCurrentScene, 188) + 8);
                        const int base = GetBaseOffset8(
                            CSF_AT(void*, g_pCurrentScene, 192), nullptr);
                        SceneLoader_ProcessSceneObjects(self, nullptr, base);
                    }
                    ++sceneIndex;
                    entryOffset += 88;
                }

                SceneLoader_UpdateLoadingProgress(self);
                SceneLoader_FinalizeWorld();
                SceneLoader_UpdateLoadingProgress(self);

                SceneLoader_PrepareResources(SceneLoader_Pointer(GetValue8B0554()), nullptr);
                void* resourceProvider =
                    CSF_AT(void*, SceneLoader_Pointer(GetValue8B0554()), 8148);

                if (CSF_AT(uint8_t,
                           CSF_AT(void*, SceneLoader_Pointer(GetValue8B0554()), 8156), 44))
                {
                    const int resourceId =
                        CSF_AT(int, SceneLoader_Pointer(GetGlobalValue_8BC1E0()), 2308);
                    const int resource =
                        ((int (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, resourceProvider, 0)[16 / 4])(
                                resourceProvider, nullptr, resourceId);
                    SceneLoader_RegisterResource(
                        SceneLoader_Pointer(GetValue8B0554()), nullptr, resource);
                }

                const int resourceId =
                    CSF_AT(int, SceneLoader_Pointer(GetGlobalValue_8BC1E0()), 2312);
                const int resource =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, resourceProvider, 0)[16 / 4])(
                            resourceProvider, nullptr, resourceId);
                SceneLoader_RegisterResource(
                    SceneLoader_Pointer(GetValue8B0554()), nullptr, resource);

                void* accessor = SceneLoader_Pointer(GlobalInt_ResolveAccessor(
                    SceneLoader_Pointer(GetGlobalInt()), nullptr));
                void* subsystem =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, accessor, 0)[20 / 4])(accessor, nullptr);
                ((void (__fastcall*)(void*, void*))
                    CSF_AT(void**, subsystem, 0)[20 / 4])(subsystem, nullptr);

                SceneLoader_ApplySceneObjectSettings(
                    SceneLoader_Pointer(GetValue8B0554()), nullptr);

                service = SceneLoader_ResolveSceneService();
                const int materialWorld =
                    ((int (__fastcall*)(void*, void*))
                        CSF_AT(void**, service, 0)[68 / 4])(service, nullptr);
                SceneLoader_PrepareWorldMaterials(materialWorld);

                service = SceneLoader_ResolveSceneService();
                void* renderWorld =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, service, 0)[68 / 4])(service, nullptr);
                const int world = SceneLoader_GetRenderWorld(renderWorld, nullptr);

                CSF_AT(uint8_t, materialContext, 0) = 0;
                SceneLoader_FillMaterialContext(materialContext);
                RpWorldForAllMaterials(
                    world,
                    reinterpret_cast<int (__cdecl*)(uint32_t, int)>(
                        CSF_ADDR_FN(0x004FEE70)),
                    reinterpret_cast<int>(materialContext));
                RpWorldForAllClumps(
                    world,
                    reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
                        CSF_ADDR_FN(0x004FF000)),
                    reinterpret_cast<int>(materialContext));

                service = SceneLoader_ResolveSceneService();
                void* referencedWorldOwner =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, service, 0)[64 / 4])(service, nullptr);
                void* referencedWorld = GetReferencedObject(referencedWorldOwner, nullptr);

                CSF_AT(uint8_t, materialContext, 0) = 1;
                SceneLoader_FillMaterialContext(materialContext);
                RpWorldForAllMaterials(
                    reinterpret_cast<int>(referencedWorld),
                    reinterpret_cast<int (__cdecl*)(uint32_t, int)>(
                        CSF_ADDR_FN(0x004FEE70)),
                    reinterpret_cast<int>(materialContext));
                RpWorldForAllClumps(
                    reinterpret_cast<int>(referencedWorld),
                    reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
                        CSF_ADDR_FN(0x004FF000)),
                    reinterpret_cast<int>(materialContext));

                SceneLoader_NotifySceneService();
                SceneLoader_FinalizeWorldObjects();
            }

            SceneLoader_FinalizeSceneSubsystem(
                SceneLoader_Pointer(GetGlobalValue_4FBDE0()), nullptr);

            sceneIndex = 0;
            entryOffset = 0;
            while (SceneLoader_HasSceneEntry(self, sceneIndex))
            {
                if (SceneLoader_IsSceneEntryActive(self, entryOffset))
                {
                    const int scene = SceneLoader_GetScene(sceneIndex);
                    SceneLoader_SelectScene(sceneIndex, 0);
                    SceneLoader_SerializeSceneObject(scene, archive);
                }
                ++sceneIndex;
                entryOffset += 88;
            }

            if (mainScenePresent)
            {
                SceneLoader_SerializeMainScene(archive);
                CSF_AT(uint8_t, self, 0x104) = 1;
            }
        }
    }

    if (SceneLoader_BeginArchiveField(archive, true))
        SceneLoader_SerializeReference(object + 228, reinterpret_cast<bool*>(archive));

    if (CSF_AT(uint8_t, archive, 0x14))
    {
        if (CSF_AT(uint8_t, archive, 0) == 0)
            SceneLoader_SelectScene(CSF_AT(int, self, 0x0C), 0);
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0)
    {
        if (CSF_AT(uint8_t, archive, 0) == 0)
            SceneLoader_RegisterArchivedObject(
                CSF_AT(void*, archive, 0x10), nullptr, reinterpret_cast<int>(self));
    }

    if (mainScenePresent)
        SceneLoader_SelectMainScene();

    const bool readyToFinish = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = materialContext[3];
    if (readyToFinish)
    {
        void* backend = CSF_AT(void*, archive, 4);
        if (((bool (__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, backend, 0)[124 / 4])(backend, nullptr, archive))
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x005053F0, CSceneLoaderBase_SerializeScene, "auto");
