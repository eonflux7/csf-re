#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, HudUpdateSceneMenuVisibility, 0x0042FA50, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneForLoad, 0x0044F380, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearSceneObjects, 0x0044F690, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinishSceneCleanup, 0x0044FB50, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SelectSceneResources, 0x00450630, (int a1, int a2));
CSF_FUNC(void, __fastcall, SceneLoader_DetachSceneObject, 0x004530D0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, SceneLoader_SetSceneActive, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSceneManager, 0x004FC550, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_RefreshSceneManager, 0x004FCB90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_BeginSceneTransition, 0x004FE880, ());
CSF_FUNC(int, __fastcall, SceneLoader_FinishLoadedScene, 0x004FFE00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_SaveSceneObject, 0x005003A0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, SceneLoader_SetLoadOption, 0x00501AB0, (void* a1, int a2, char a3));
CSF_FUNC(char, __fastcall, SceneLoader_LoadSceneFile, 0x00503000, (void* self, void* edx, char* a2, int a4, int a5));
CSF_FUNC(int, __fastcall, SceneLoader_RestoreStandardScene, 0x005041D0, (C_0082d410* self, void* edx, int sceneIndex, C_00834050* context, int firstObject, int secondObject, int* position, int heading));
CSF_FUNC(int, __fastcall, SceneLoader_RestoreAlternateScene, 0x005043F0, (void* self, void* edx, int a1, void* a3, int a4, int a5, int* a6, int a7));
CSF_FUNC(int*, __cdecl, SceneLoader_ResetLoadResources, 0x00504570, ());
CSF_FUNC(int, __fastcall, SceneLoader_SaveDetachedObjects, 0x00506180, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, SceneLoader_GetRestoredObjects, 0x00506370, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(void, __fastcall, SceneLoader_ResetSceneSubsystem, 0x00519C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_CollectUnusedResources, 0x00691080, ());
CSF_FUNC(std::size_t, __cdecl, SceneLoader_UpdateMemoryAccounting, 0x00691090, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(char, __cdecl, SceneLoader_ClosePackage, 0x006980D0, (char* String1));
CSF_FUNC(char, __cdecl, SceneLoader_SetPackageLoadingEnabled, 0x00698600, (char a1));
CSF_FUNC(void, __cdecl, SceneLoader_OpenPackage, 0x00698960, (char* String1));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SceneLoader_LoadTextDatabase, 0x0069BD90, (void* self, void* edx, unsigned int a2, char* a3));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadScope, 0x0069CDC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFpsStatsFromGlobal, 0x006B2B50, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_RefreshEntryLookup, 0x006DB5D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// The concrete loader layout is accessed through its declared base.
static int __fastcall CSceneLoader_LoadScene(
    C_0082d410* self, void* /*edx*/, int sceneIndex, int /*unused*/)
{
    SceneLoader_BeginSceneTransition();

    // These buffers retain the full room available to the original stack objects.
    char packagePath[512];
    uint32_t notification[14] = {};
    int savedPosition[5] = {};
    uint32_t secondObjectStorage[2] = {};
    uint32_t firstObject = 0;
    uint32_t menuContext = 0;

    uint8_t* loadScope = SceneLoader_CreateLoadScope();
    notification[13] = reinterpret_cast<uint32_t>(loadScope);
    menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);

    if (menuContext)
    {
        SceneLoader_SaveSceneObject(
            self, nullptr, CSF_AT(int, self, 0x0C),
            reinterpret_cast<uint32_t*>(menuContext));

        int* position = reinterpret_cast<int*>(
            GetObjectPositionVector(reinterpret_cast<void*>(menuContext), nullptr));
        savedPosition[0] = position[0];
        savedPosition[1] = position[1];
        savedPosition[2] = position[2];

        auto* context = reinterpret_cast<C_008342e8*>(menuContext);
        void* controller = reinterpret_cast<void*>(context->vGetField016c());
        void* orientation = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x18 / 4])(controller, nullptr);
        float heading = static_cast<float>(((double(__fastcall*)(void*, void*))
            CSF_AT(void**, orientation, 0)[0x28 / 4])(orientation, nullptr));
        std::memcpy(&savedPosition[3], &heading, sizeof(heading));

        SceneLoader_DetachSceneObject(g_pCurrentScene, nullptr, nullptr);
    }

    uint8_t* objectList = static_cast<uint8_t*>(g_pCurrentScene) + 0x60;
    uint32_t comparedContext = menuContext;
    uint32_t objectIndex = 0;

    if (CSF_AT(uint32_t, objectList, 0x08) > 0)
    {
        do
        {
            uint32_t object = reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, objectList, 0))[objectIndex];

            if (!object || object == comparedContext)
            {
                ++objectIndex;
                savedPosition[4] = static_cast<int>(objectIndex);
            }
            else
            {
                if (firstObject)
                    secondObjectStorage[0] = object;
                else
                    firstObject = object;

                SceneLoader_SaveSceneObject(
                    self, nullptr, CSF_AT(int, self, 0x0C),
                    reinterpret_cast<uint32_t*>(object));
                SceneLoader_DetachSceneObject(
                    g_pCurrentScene, nullptr, reinterpret_cast<void*>(object));

                comparedContext = menuContext;
                objectIndex = static_cast<uint32_t>(savedPosition[4]);
            }
        }
        while (objectIndex < CSF_AT(uint32_t, objectList, 0x08));
    }

    SceneLoader_ClearSceneObjects(g_pCurrentScene, nullptr);
    SceneLoader_SetSceneActive(g_pCurrentScene, nullptr, 0);
    SceneLoader_FinishSceneCleanup(g_pCurrentScene, nullptr);
    SceneLoader_SaveDetachedObjects(
        self, nullptr, static_cast<int>(menuContext),
        static_cast<int>(firstObject), static_cast<int>(secondObjectStorage[0]));

    if (menuContext)
    {
        reinterpret_cast<C_0082d410*>(menuContext)->vSlot0(1);
        menuContext = 0;
    }
    if (firstObject)
    {
        void* object = reinterpret_cast<void*>(firstObject);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        firstObject = 0;
    }
    if (secondObjectStorage[0])
    {
        void* object = reinterpret_cast<void*>(secondObjectStorage[0]);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        secondObjectStorage[0] = 0;
    }

    SceneLoader_ResetSceneManager(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
    SceneLoader_SelectSceneResources(sceneIndex, 0);
    ((void(__fastcall*)(C_0082d410*, void*, int))
        self->vtbl[0x3C / 4])(self, nullptr, 1);

    uint32_t sceneEntries = CSF_AT(uint32_t, self, 0x30);
    CSF_AT(int, self, 0x14) = sceneIndex;
    uint8_t* requestedSceneName = reinterpret_cast<uint8_t*>(self) + 0x7C;
    AssignStringSubstring(
        requestedSceneName, nullptr,
        reinterpret_cast<void*>(sceneEntries + 88 * sceneIndex), 0, -1);

    SceneLoader_SetPackageLoadingEnabled(0);
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 2652))
    {
        SceneLoader_ClosePackage(reinterpret_cast<char*>(GetGlobalIntPtr()));
        const char* sceneName = CSF_AT(uint32_t, self, 0x94) < 0x10
            ? reinterpret_cast<char*>(self) + 0x80
            : reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x80));
        std::strcpy(packagePath, sceneName);
        Path_ReplaceExtension(
            packagePath, 512, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
        Nullsub_506AC0();
        SceneLoader_OpenPackage(packagePath);
    }

    SceneLoader_PrepareSceneForLoad(g_pCurrentScene, nullptr);
    CSF_AT(int, self, 0x5C) = 2;
    SceneLoader_SetLoadOption(self, 0, 0);
    void* sceneSubsystem = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 8132);
    SceneLoader_ResetSceneSubsystem(sceneSubsystem, nullptr);
    SceneLoader_ResetLoadResources();

    char* sceneName = CSF_AT(uint32_t, self, 0x94) < 0x10
        ? reinterpret_cast<char*>(self) + 0x80
        : reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x80));
    char loaded = SceneLoader_LoadSceneFile(self, nullptr, sceneName, 0, 0);

    if (loaded)
    {
        SceneLoader_FinishLoadedScene(self, nullptr);
        void* renderController = reinterpret_cast<void*>(
            SceneLoader_GetRenderController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, renderController, 0)[0x2C / 4])(
                renderController, nullptr);

        SceneLoader_RefreshSceneManager(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
        SceneLoader_GetRestoredObjects(
            self, nullptr, &menuContext, &firstObject, secondObjectStorage);

        uint8_t* sceneEntry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x30) + 88 * sceneIndex);
        if (CSF_AT(uint8_t, sceneEntry, 29))
        {
            SceneLoader_RestoreAlternateScene(
                self, nullptr, sceneIndex, reinterpret_cast<void*>(menuContext),
                static_cast<int>(firstObject),
                static_cast<int>(secondObjectStorage[0]),
                savedPosition, savedPosition[3]);
        }
        else
        {
            SceneLoader_RestoreStandardScene(
                self, nullptr, sceneIndex,
                reinterpret_cast<C_00834050*>(menuContext),
                static_cast<int>(firstObject),
                static_cast<int>(secondObjectStorage[0]),
                savedPosition, savedPosition[3]);
        }
    }

    if (loadScope)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, loadScope, 0)[0x10 / 4])(loadScope, nullptr);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, loadScope, 0)[0])(loadScope, nullptr, 1);
    }

    uint8_t* currentSceneName = reinterpret_cast<uint8_t*>(self) + 0x60;
    if (loaded)
    {
        AssignStringSubstring(
            currentSceneName, nullptr, requestedSceneName, 0, -1);
        CSF_AT(int, self, 0x5C) = 3;
    }
    else
    {
        AssignStringRange_429B60(
            currentSceneName, nullptr,
            reinterpret_cast<char*>(&g_byte_82E65C), 0);
        CSF_AT(int, self, 0x5C) = 0;
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 2652))
    {
        const char* packageSceneName = CSF_AT(uint32_t, self, 0x94) < 0x10
            ? reinterpret_cast<char*>(self) + 0x80
            : reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x80));
        std::strcpy(packagePath, packageSceneName);
        Path_ReplaceExtension(
            packagePath, 512, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
        SceneLoader_ClosePackage(packagePath);
        SceneLoader_OpenPackage(reinterpret_cast<char*>(GetGlobalIntPtr()));
    }

    uint8_t* sceneSettings = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 84;
    uint8_t* textDatabaseName = sceneSettings + 5612;
    if (CompareStringSubstring(
            textDatabaseName, nullptr, 0,
            CSF_AT(unsigned int, sceneSettings, 5632),
            reinterpret_cast<char*>(&g_byte_82E65C), 0))
    {
        char* textName = CSF_AT(uint32_t, textDatabaseName, 24) < 0x10u
            ? reinterpret_cast<char*>(textDatabaseName + 4)
            : CSF_AT(char*, textDatabaseName, 4);
        SceneLoader_LoadTextDatabase(
            reinterpret_cast<void*>(GetGlobalValue_69B700()), nullptr, 4u, textName);
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 2652))
        SceneLoader_ClosePackage(reinterpret_cast<char*>(GetGlobalIntPtr()));

    SceneLoader_SetPackageLoadingEnabled(1);
    CSF_AT(int, self, 0x0C) = sceneIndex;
    CSF_AT(int, self, 0x14) = -1;

    int clumpManager = ResolveClumpManager(GetGlobalInt(), nullptr);
    int entryLookup = ResolveEntryLookupValue(
        reinterpret_cast<void*>(clumpManager), nullptr);
    SceneLoader_RefreshEntryLookup(reinterpret_cast<void*>(entryLookup), nullptr);

    void* fpsStats = reinterpret_cast<void*>(
        GetFpsStatsFromGlobal(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, fpsStats, 0)[0x08 / 4])(fpsStats, nullptr);

    notification[2] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    notification[5] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    notification[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
    notification[3] = -1;
    notification[4] = -1;
    notification[6] = -1;
    notification[7] = -1;
    notification[1] = 36;
    std::memset(&notification[8], 0, 16);
    reinterpret_cast<uint8_t*>(notification)[0x30] = 0;
    DispatchRecordToGlobalObject(
        reinterpret_cast<void*>(GetGlobalObjectField184()),
        nullptr, notification, 1);

    SceneLoader_UpdateMemoryAccounting();
    SceneLoader_CollectUnusedResources();

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* sceneObserver = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, globalObject, 0)[112 / 4])(globalObject, nullptr);
    if (sceneObserver)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObserver, 0)[64 / 4])(sceneObserver, nullptr);
    }

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* accessorObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[16 / 4])(accessor, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, accessorObject, 0)[40 / 4])(accessorObject, nullptr);

    return HudUpdateSceneMenuVisibility(
        reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);
}

HOOK(0x00506600, CSceneLoader_LoadScene, "auto");
