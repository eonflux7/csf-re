#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneLoader_ResetSceneObjects, 0x004448A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FindFocusEntry, 0x0044FBF0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneLoader_ResetSceneRegions, 0x00469070, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(char, __fastcall, SceneLoader_PrepareRegionManager, 0x004FBE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ResetRegionManager, 0x004FCB90, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_PrepareSceneResources, 0x004FF160, ());
CSF_FUNC(int, __cdecl, SceneLoader_PrepareSceneLoading, 0x004FFAB0, ());
CSF_FUNC(int, __cdecl, SceneLoader_FinalizeSceneLoading, 0x00500270, ());
CSF_FUNC(void*, __stdcall, SceneLoader_LoadRegion, 0x005004E0, (int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetRuntimeInterface, 0x00652010, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_ServiceLoading, 0x0069AF70, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall CSceneLoader_LoadSelectedRegions(
    C_0082d410* self, void* /*edx*/,
    int /*unused*/, char selective, int selection,
    std::uint32_t* matchingRegion, std::uint32_t* firstOtherRegion,
    std::uint32_t* nextOtherRegion, int deferFirstOther, int deferNextOther)
{
    void* regionLookup = CSF_AT(void*, GetValue8B0554(), 0x1FC8);
    auto* sceneSettings =
        reinterpret_cast<std::uint8_t*>(GetValue8B0554()) + 0x54;

    if (!selective)
    {
        void* runtime = reinterpret_cast<void*>(
            SceneLoader_GetRuntimeInterface(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, runtime, 0)[0x2C / 4])(runtime, nullptr);
    }

    SceneLoader_PrepareSceneLoading();
    SceneLoader_PrepareSceneResources();
    SceneLoader_PrepareRegionManager(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
    if (!selective)
    {
        SceneLoader_ResetRegionManager(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
    }

    SceneLoader_ResetSceneRegions(
        CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr);
    SceneLoader_ResetSceneObjects(
        CSF_AT(void*, g_pCurrentScene, 0xC0), nullptr);

    const std::uint32_t regionCount =
        CSF_AT(std::uint32_t, GetValue8B0554(), 0x0C);
    // The unsigned conversion replaces the original signed fild plus bias.
    const float regionProgress =
        g_fReturnedConstant_0082E030 / static_cast<float>(regionCount);

    for (std::uint32_t index = 0; index < regionCount; ++index)
    {
        auto* region = reinterpret_cast<std::uint32_t*>(
            GetSceneRegionAt(reinterpret_cast<void*>(GetValue8B0554()),
                             nullptr, static_cast<int>(index)));

        const int entryIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, regionLookup, 0)[0x10 / 4])(
                    regionLookup, nullptr, region[0]);
        void* entry = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, regionLookup, 0)[0x08 / 4])(
                    regionLookup, nullptr, entryIndex));

        bool loadRegion = true;
        if (selective && !CSF_AT(std::uint32_t, entry, 0xB4))
        {
            if (!selection ||
                CSF_AT(std::uint32_t, selection, 0x54) ==
                    CSF_AT(std::uint32_t, entry, 0xB8))
            {
                *matchingRegion = region[1];
                loadRegion = false;
            }
            else if (*firstOtherRegion == static_cast<std::uint32_t>(-1))
            {
                *firstOtherRegion = region[1];
                loadRegion = !deferFirstOther;
            }
            else
            {
                *nextOtherRegion = region[1];
                loadRegion = !deferNextOther;
            }
        }

        if (loadRegion)
        {
            SceneLoader_LoadRegion(static_cast<int>(index));
            if (CSF_AT(std::uint8_t, self, 0xB6) &&
                CSF_AT(void*, GetGlobalState_50D190(), 0x1C))
            {
                void* progress =
                    CSF_AT(void*, GetGlobalState_50D190(), 0x1C);
                reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                    CSF_AT(void**, progress, 0)[0x44 / 4])(
                        progress, nullptr, regionProgress);
            }
            SceneLoader_ServiceLoading();
        }
    }

    const int focusEntry = SceneLoader_FindFocusEntry(
        g_pCurrentScene, nullptr, CSF_AT(int, sceneSettings, 0x10));
    if (focusEntry)
        CurrentScene_SetFocusEntry(g_pCurrentScene, nullptr, focusEntry);

    if (CSF_AT(std::uint8_t, self, 0xB6) &&
        CSF_AT(void*, GetGlobalState_50D190(), 0x1C))
    {
        void* progress = CSF_AT(void*, GetGlobalState_50D190(), 0x1C);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, progress, 0)[0x44 / 4])(
                progress, nullptr, 10.0f);
    }

    CSF_AT(std::uint8_t, g_pCurrentScene, 0x1A9) =
        CSF_AT(std::uint8_t, sceneSettings, 0x91D);
    SceneLoader_FinalizeSceneLoading();
    return 1;
}

HOOK(0x00500AE0, CSceneLoader_LoadSelectedRegions, "auto");
