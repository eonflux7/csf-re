#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_PrepareResourceManager, 0x0043FBA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSceneManager, 0x00442A00, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SetSceneOptions, 0x00450630, (int a1, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_ResetSceneContext, 0x004506B0, ());
CSF_FUNC(char, __fastcall, SceneLoader_PrepareCurrentScene, 0x00455240, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, SceneLoader_ResetSceneResources, 0x004565D0, ());
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, CSceneLoader_FinishBaseMapLoad, 0x004FFE00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CSceneLoader_UpdateLoadedScene, 0x00500460, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CSceneLoader_FinishSceneDataLoad, 0x00500800, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSceneLoader_LoadSceneData, 0x00500AE0, (void* self, void* edx, int a2, char a3, int a4, uint32_t* a5, uint32_t* a6, uint32_t* a7, int a8, int a9));
CSF_FUNC(int, __fastcall, CSceneLoader_PrepareAlternateMap, 0x00501230, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSceneLoader_FinishAlternateMapLoad, 0x00502250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSceneLoader_LoadBaseMap, 0x00503000, (void* self, void* edx, char* a2, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, CSceneLoader_LoadMapResources, 0x00503BF0, (void* self, void* edx, char* source));
CSF_FUNC(int, __fastcall, CSceneLoader_FinishMapResources, 0x00503EF0, (void* self, void* edx));
CSF_FUNC(int*, __cdecl, SceneLoader_PrepareMapLoading, 0x00504570, ());
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, SceneLoader_PrepareLoadInterface, 0x00519C30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, SceneLoader_SetLoadContextOptions, 0x00521F40, (int a1, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_FinishLoadContext, 0x005226A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_FinishMemoryMaintenance, 0x00691080, ());
CSF_FUNC(std::size_t, __cdecl, SceneLoader_BeginMemoryMaintenance, 0x00691090, ());
CSF_FUNC(void, __cdecl, SetFallbackContext, 0x006912F0, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(char, __cdecl, SceneLoader_ProcessResourcePath, 0x006980D0, (char* String1));
CSF_FUNC(void, __cdecl, SceneLoader_UpdateResourcePath, 0x00698960, (char* String1));
CSF_FUNC(int, __fastcall, SceneLoader_ReleasePendingResource, 0x0069AEF0, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SceneLoader_SetTextDatabasePath, 0x0069BD90, (void* self, void* edx, unsigned int a2, char* a3));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFpsStatsFromGlobal, 0x006B2B50, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_FinishClumpLookup, 0x006DB5D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(void*, g_PendingSceneResource, 0x008BAF90);

static char* CSceneLoader_GetMapPath(C_0082d410* self)
{
    if (CSF_AT(uint32_t, self, 0x94) < 0x10)
        return reinterpret_cast<char*>(self) + 0x80;
    return reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x80));
}

static int CSceneLoader_CompareAlternateMapWithEmpty(C_0082d410* self)
{
    return CompareStringSubstring(
        reinterpret_cast<uint8_t*>(self) + 0x98, nullptr,
        0, CSF_AT(uint32_t, self, 0xAC),
        CSF_STR(0x0082E65C, ""), 0);
}

static char __fastcall CSceneLoader_LoadMap(C_0082d410* self, void* /*edx*/)
{
    char loaded = 1;

    SceneLoader_PrepareResourceManager(
        reinterpret_cast<void*>(GetGlobal8afc7c()), nullptr);
    CSF_AT(uint32_t, self, 0x5C) = 2;

    // The second dword shares storage with the timer pointer in the original.
    uint32_t sceneResults[2] = {
        0, reinterpret_cast<uint32_t>(SceneLoader_CreateLoadTimer())
    };
    uint32_t sceneResult = 0;
    uint32_t sceneIndex = 0;

    if (!CSF_AT(uint8_t, self, 0xE0))
    {
        void* settings = reinterpret_cast<void*>(GetValue8B0554());
        SceneLoader_PrepareLoadInterface(
            CSF_AT(void*, settings, 0x1FC4), nullptr);
    }

    if (CSceneLoader_CompareAlternateMapWithEmpty(self))
    {
        CSceneLoader_PrepareAlternateMap(self, nullptr);
        if (!CSF_AT(uint8_t, self, 0xE0))
        {
            if (((bool(__fastcall*)(void*, void*))self->vtbl[0x28 / 4])(self, nullptr)
                && (SceneLoader_PrepareMapLoading(),
                    ((bool(__fastcall*)(void*, void*))self->vtbl[0x28 / 4])(self, nullptr))
                && CSceneLoader_LoadBaseMap(
                    self, nullptr, CSceneLoader_GetMapPath(self), 0, 0)
                && ((bool(__fastcall*)(void*, void*))self->vtbl[0x28 / 4])(self, nullptr))
            {
                CSceneLoader_FinishBaseMapLoad(self, nullptr);
            }
            else
            {
                loaded = 0;
            }
        }

        SceneLoader_ResetSceneManager(
            reinterpret_cast<void*>(GetValue_8afc80()), nullptr);
        SceneLoader_PrepareCurrentScene(g_pCurrentScene, nullptr);
        SetFallbackContext();

        void* sceneContext = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* world = CSF_AT(void*, sceneContext, 0x4);
        if (loaded)
        {
            loaded = static_cast<char>(
                ((int(__fastcall*)(void*, void*, uint8_t*))
                    CSF_AT(void**, world, 0)[0x28 / 4])(
                        world, nullptr, reinterpret_cast<uint8_t*>(self) + 0x98));
            GetGlobalValue_521BB0();
            SceneLoader_SetLoadContextOptions(1, 0);
            SceneLoader_FinishLoadContext(
                reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);
        }
    }
    else
    {
        if (!CSF_AT(uint8_t, self, 0xE0))
        {
            SceneLoader_ResetSceneContext();
            SceneLoader_ResetSceneResources();
            CSF_AT(uint32_t, self, 0xC) = 0;
            SceneLoader_SetSceneOptions(0, 0);
            SceneLoader_PrepareMapLoading();
            loaded = CSceneLoader_LoadBaseMap(
                self, nullptr, CSceneLoader_GetMapPath(self), 0, 0);
            if (loaded)
                CSceneLoader_FinishBaseMapLoad(self, nullptr);
        }

        if (loaded)
        {
            sceneIndex = static_cast<uint32_t>(-1);
            SceneLoader_ResetSceneManager(
                reinterpret_cast<void*>(GetValue_8afc80()), nullptr);
            SceneLoader_PrepareCurrentScene(g_pCurrentScene, nullptr);
            loaded = CSceneLoader_LoadSceneData(
                self, nullptr, reinterpret_cast<int>(CSceneLoader_GetMapPath(self)),
                0, 0, &sceneResult, &sceneIndex, sceneResults, 0, 0);
            CSceneLoader_FinishSceneDataLoad(self, nullptr);
        }
    }

    void* timer = reinterpret_cast<void*>(sceneResults[1]);
    if (timer)
    {
        ((float(__fastcall*)(void*, void*))
            CSF_AT(void**, timer, 0)[0x10 / 4])(timer, nullptr);
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, timer, 0)[0])(timer, nullptr, 1);
    }

    ((void(__fastcall*)(void*, void*, float))self->vtbl[0x40 / 4])(
        self, nullptr, 220.0f);

    if (loaded)
    {
        AssignStringSubstring(
            reinterpret_cast<uint8_t*>(self) + 0x60, nullptr,
            reinterpret_cast<uint8_t*>(self) + 0x7C, 0,
            static_cast<uint32_t>(-1));
        CSF_AT(uint32_t, self, 0x5C) = 3;
    }
    else
    {
        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x60, nullptr,
            const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);
        CSF_AT(uint32_t, self, 0x5C) = 0;
    }

    Nullsub_506AC0();
    if (CSF_AT(uint32_t, self, 0x5C) == 3)
    {
        CSceneLoader_LoadMapResources(self, nullptr, CSceneLoader_GetMapPath(self));
        CSceneLoader_FinishMapResources(self, nullptr);
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C)
        && (!CSF_AT(uint8_t, self, 0xE0)
            || !CSceneLoader_CompareAlternateMapWithEmpty(self)))
    {
        char archivePath[512];
        std::strcpy(archivePath, CSceneLoader_GetMapPath(self));
        Path_ReplaceExtension(
            archivePath, 512, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
        SceneLoader_ProcessResourcePath(archivePath);
        SceneLoader_UpdateResourcePath(reinterpret_cast<char*>(GetGlobalIntPtr()));
    }

    if (!CSF_AT(uint8_t, self, 0xE0)
        || !CSceneLoader_CompareAlternateMapWithEmpty(self))
    {
        uint8_t* settings = reinterpret_cast<uint8_t*>(GetValue8B0554());
        uint8_t* textPath = settings + 5696;
        if (CompareStringSubstring(
                textPath, nullptr, 0, CSF_AT(uint32_t, settings, 5716),
                CSF_STR(0x0082E65C, ""), 0))
        {
            char* path;
            if (CSF_AT(uint32_t, textPath, 24) < 0x10u)
                path = reinterpret_cast<char*>(textPath + 4);
            else
                path = CSF_AT(char*, textPath, 4);

            SceneLoader_SetTextDatabasePath(
                reinterpret_cast<void*>(GetGlobalValue_69B700()), nullptr, 4u, path);
        }
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
        SceneLoader_ProcessResourcePath(reinterpret_cast<char*>(GetGlobalIntPtr()));

    int manager = GetGlobalInt();
    int clumpManager = ResolveClumpManager(manager, nullptr);
    int lookup = ResolveEntryLookupValue(reinterpret_cast<void*>(clumpManager), nullptr);
    SceneLoader_FinishClumpLookup(reinterpret_cast<void*>(lookup), nullptr);

    void* stats = reinterpret_cast<void*>(
        GetFpsStatsFromGlobal(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, stats, 0)[0x8 / 4])(stats, nullptr);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateScene = CSF_AT(void*, state, 0x10);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, stateScene, 0)[0x70 / 4])(stateScene, nullptr, 0);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, state, 0)[0x5C / 4])(state, nullptr, 0);

    SceneLoader_BeginMemoryMaintenance();
    SceneLoader_FinishMemoryMaintenance();

    void* globalContext = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* contextObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, globalContext, 0)[0x70 / 4])(globalContext, nullptr);
    if (contextObject)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, contextObject, 0)[0x40 / 4])(contextObject, nullptr);
    }

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* accessorObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, accessorObject, 0)[0x28 / 4])(accessorObject, nullptr);

    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
        CSceneLoader_UpdateLoadedScene(self, nullptr);

    if (g_PendingSceneResource)
    {
        SceneLoader_ReleasePendingResource(g_PendingSceneResource, nullptr);
        g_PendingSceneResource = nullptr;
    }

    CSF_AT(uint8_t, self, 0xB6) = 0;
    GetGlobalValue_8B056C();
    g_PendingSceneResource = nullptr;
    if (CSceneLoader_CompareAlternateMapWithEmpty(self))
        CSceneLoader_FinishAlternateMapLoad(self, nullptr);

    return loaded;
}

HOOK(0x00504970, CSceneLoader_LoadMap, "auto");
