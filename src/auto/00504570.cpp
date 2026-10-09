#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, SceneLoader_RefreshSceneWeather, 0x0044F380, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (void* self, void* edx, void* name));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_RefreshWeatherResources, 0x004E8C80, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, SceneLoader_DestroyWeatherEffectLists, 0x00502E00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_InitializeWeatherEffectLists, 0x00503570, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_00867260_GetWeatherController, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_psz_PFX_RAIN_FX_FIRST_PLANE, 0x008A8C54);
CSF_GLOBAL(const char*, g_psz_PFX_RAIN_FX_BACK_PLANE, 0x008A8C58);
CSF_GLOBAL(const char*, g_psz_PFX_RAIN_FX_NEAR_1, 0x008A8C5C);
CSF_GLOBAL(const char*, g_psz_PFX_RAIN_FX_FAR_1, 0x008A8C64);
CSF_GLOBAL(const char*, g_psz_PFX_SNOW_FX_FIRST_PLANE, 0x008A8C68);
CSF_GLOBAL(const char*, g_psz_PFX_SNOW_FX_BACK_PLANE, 0x008A8C6C);
CSF_GLOBAL(const char*, g_psz_PFX_SNOW_FX_NEAR_1, 0x008A8C70);
CSF_GLOBAL(const char*, g_psz_PFX_SNOW_FX_FAR_1, 0x008A8C7C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int SceneLoader_FindWeatherEffect(void* definitions, const char* name)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, definitions, 0)[0xC / 4])(definitions, nullptr, name);
}

static void* SceneLoader_GetWeatherEffect(void* definitions, int index)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, definitions, 0)[0x8 / 4])(definitions, nullptr, index);
}

static int __cdecl SceneLoader_ConfigureWeatherEffects()
{
    void* owner = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, owner, 0x1FCC);

    // Nine adjacent list objects, including the unused middle list.
    // Keep the entire original frame available to the initializing callee.
    alignas(4) uint8_t effectLists[0x9C];
    SceneLoader_InitializeWeatherEffectLists(effectLists, nullptr);

    int index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_RAIN_FX_FIRST_PLANE);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        void* name = static_cast<uint8_t*>(entry) + 8;
        if (CompareStringSubstring(
                name, nullptr, 0, CSF_AT(unsigned int, entry, 0x1C),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0)) {
            MenuManager_AppendPageName(effectLists, nullptr, name);
        }
    }

    index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_RAIN_FX_BACK_PLANE);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        void* name = static_cast<uint8_t*>(entry) + 8;
        if (CompareStringSubstring(
                name, nullptr, 0, CSF_AT(unsigned int, entry, 0x1C),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0)) {
            MenuManager_AppendPageName(effectLists + 0x30, nullptr, name);
        }
    }

    const char** effectName = &g_psz_PFX_RAIN_FX_NEAR_1;
    do {
        index = SceneLoader_FindWeatherEffect(definitions, *effectName);
        if (index != -1) {
            void* entry = SceneLoader_GetWeatherEffect(definitions, index);
            void* name = static_cast<uint8_t*>(entry) + 8;
            if (CompareStringSubstring(
                    name, nullptr, 0, CSF_AT(unsigned int, entry, 0x1C),
                    reinterpret_cast<const char*>(&g_byte_82E65C), 0)) {
                if (!CSF_AT(uint32_t, entry, 0x4) &&
                    !CSF_AT(uint8_t, entry, 0x50)) {
                    MenuManager_AppendPageName(
                        effectLists + 0x10, nullptr, name);
                }
            }
        }
        ++effectName;
    } while (reinterpret_cast<std::intptr_t>(effectName) <
             reinterpret_cast<std::intptr_t>(&g_psz_PFX_RAIN_FX_FAR_1));

    index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_RAIN_FX_FAR_1);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        if (CSF_AT(uint32_t, entry, 0x1C) &&
            !CSF_AT(uint32_t, entry, 0x4) &&
            !CSF_AT(uint8_t, entry, 0x50)) {
            MenuManager_AppendPageName(
                effectLists + 0x20, nullptr, static_cast<uint8_t*>(entry) + 8);
        }
    }

    index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_SNOW_FX_FIRST_PLANE);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        if (CSF_AT(uint32_t, entry, 0x1C)) {
            MenuManager_AppendPageName(
                effectLists + 0x50, nullptr, static_cast<uint8_t*>(entry) + 8);
        }
    }

    index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_SNOW_FX_BACK_PLANE);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        if (CSF_AT(uint32_t, entry, 0x1C)) {
            MenuManager_AppendPageName(
                effectLists + 0x80, nullptr, static_cast<uint8_t*>(entry) + 8);
        }
    }

    effectName = &g_psz_PFX_SNOW_FX_NEAR_1;
    do {
        index = SceneLoader_FindWeatherEffect(definitions, *effectName);
        if (index != -1) {
            void* entry = SceneLoader_GetWeatherEffect(definitions, index);
            if (CSF_AT(uint32_t, entry, 0x1C) &&
                !CSF_AT(uint32_t, entry, 0x4) &&
                !CSF_AT(uint8_t, entry, 0x50)) {
                MenuManager_AppendPageName(
                    effectLists + 0x60, nullptr,
                    static_cast<uint8_t*>(entry) + 8);
            }
        }
        ++effectName;
    } while (reinterpret_cast<std::intptr_t>(effectName) <
             reinterpret_cast<std::intptr_t>(&g_psz_PFX_SNOW_FX_FAR_1));

    index = SceneLoader_FindWeatherEffect(
        definitions, g_psz_PFX_SNOW_FX_FAR_1);
    if (index != -1) {
        void* entry = SceneLoader_GetWeatherEffect(definitions, index);
        if (CSF_AT(uint32_t, entry, 0x1C) &&
            !CSF_AT(uint32_t, entry, 0x4) &&
            !CSF_AT(uint8_t, entry, 0x50)) {
            MenuManager_AppendPageName(
                effectLists + 0x70, nullptr, static_cast<uint8_t*>(entry) + 8);
        }
    }

    void* globalManager = reinterpret_cast<void*>(GetGlobalInt());
    void* weatherController = reinterpret_cast<void*>(
        C_00867260_GetWeatherController(globalManager, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, weatherController, 0)[0x40 / 4])(
            weatherController, nullptr, effectLists);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    float weatherBlend = CSF_AT(uint8_t, globalState, 0xD0) ? 0.7f : 0.0f;
    int managerId = GetGlobalInt();
    void* clumpManager = reinterpret_cast<void*>(
        ResolveClumpManager(managerId, nullptr));
    void* lookup = reinterpret_cast<void*>(
        ResolveEntryLookupValue(clumpManager, nullptr));
    CSF_AT(float, lookup, 0x108) = weatherBlend;

    SceneLoader_RefreshSceneWeather(g_pCurrentScene, nullptr);
    SceneLoader_RefreshWeatherResources(
        reinterpret_cast<void*>(GetValue8B0554()), nullptr);
    return reinterpret_cast<int>(
        SceneLoader_DestroyWeatherEffectLists(effectLists, nullptr));
}

HOOK(0x00504570, SceneLoader_ConfigureWeatherEffects, "auto");
