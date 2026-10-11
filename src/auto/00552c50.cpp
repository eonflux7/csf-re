#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(C_008342e8*, __stdcall, SceneLoader_LoadRegionObject, 0x005004E0, (int regionIndex));

CSF_GLOBAL(float, g_SpawnRegionDistanceSquared, 0x0084462C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static char __fastcall MpSpawnPoints_LoadMissingSpawnRegions(
    uint8_t* self, void* /*edx*/)
{
    if (!g_CharacterSelectState8B0694)
        return 0;

    void* selectedObject = CSF_AT(
        void*, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
    if (!selectedObject || reinterpret_cast<uintptr_t>(selectedObject) == 776)
        return 0;

    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, selectedObject, 0)[0x8 / 4])(
                selectedObject, nullptr))
        return 0;

    const uint8_t alreadyLoaded = CSF_AT(uint8_t, self, 0x14AC);
    if (alreadyLoaded)
        return static_cast<char>(alreadyLoaded);

    CSF_AT(uint8_t, self, 0x14AC) = 1;

    const uint32_t regionCount =
        reinterpret_cast<C_0085e7b8*>(GetValue8B0554())->field_000c;
    void* regionRegistry =
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);

    uint32_t regionIndex = 0;
    while (regionIndex < regionCount)
    {
        void* region = reinterpret_cast<void*>(GetSceneRegionAt(
            reinterpret_cast<void*>(GetValue8B0554()), nullptr,
            static_cast<int>(regionIndex)));

        const int entryIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, regionRegistry, 0)[0x10 / 4])(
                    regionRegistry, nullptr, CSF_AT(uint32_t, region, 0));

        if (entryIndex != -1)
        {
            void* entry =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, regionRegistry, 0)[0x8 / 4])(
                        regionRegistry, nullptr, entryIndex);

            if (CSF_AT(int, entry, 0xD8) == 7)
            {
                bool foundNearby = false;
                uint32_t objectIndex = 0;
                void* scene = g_pCurrentScene;

                if (CSF_AT(uint32_t, scene, 0x18))
                {
                    do
                    {
                        if (foundNearby)
                            break;

                        C_008342e8* object =
                            CSF_AT(C_008342e8**, scene, 0x10)[objectIndex];
                        if (object)
                        {
                            C_008672e4* component =
                                reinterpret_cast<C_008672e4*>(object->field_0034);
                            if (component &&
                                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                                    CSF_AT(void**, component, 0)[0x98 / 4])(
                                        component, nullptr) == 7)
                            {
                                void* position = reinterpret_cast<void*>(
                                    GetObjectPositionVector(object, nullptr));

                                const float dx = CSF_AT(float, region, 0x2C) -
                                                 CSF_AT(float, position, 0);
                                const float dy = CSF_AT(float, region, 0x30) -
                                                 CSF_AT(float, position, 0x4);
                                const float dz = CSF_AT(float, region, 0x34) -
                                                 CSF_AT(float, position, 0x8);
                                foundNearby = dz * dz + dx * dx + dy * dy <
                                              g_SpawnRegionDistanceSquared;
                            }
                        }

                        scene = g_pCurrentScene;
                        ++objectIndex;
                    } while (objectIndex < CSF_AT(uint32_t, scene, 0x18));
                }

                if (!foundNearby)
                {
                    GetGlobalValue_8B0568();
                    SceneLoader_LoadRegionObject(static_cast<int>(regionIndex));
                }
            }
        }

        ++regionIndex;
    }

    return static_cast<char>(regionIndex);
}

HOOK(0x00552C50, MpSpawnPoints_LoadMissingSpawnRegions, "auto");
