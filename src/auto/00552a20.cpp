#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_ResetSpecialObject, 0x004D6AC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_IsSpecialObjectActive, 0x004D6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetSpawnObject, 0x0073BDA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetStatsCategory, 0x00748010, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshStats, 0x00748AE0, (void* self, void* edx));

static char __fastcall MpSpawnPoints_ResetSceneObjects(
    void* self, void* /*edx*/, char selective)
{
    auto* scene = static_cast<CScene*>(g_pCurrentScene);
    const int32_t objectCount = static_cast<int32_t>(scene->m_objectCount);

    for (int32_t index = 0; index < objectCount; ++index)
    {
        void* entry = reinterpret_cast<void**>(scene->m_objects)[index];
        if (entry)
        {
            void* object = CSF_AT(void*, entry, 0x18);
            if (object)
            {
                if (selective)
                {
                    switch (CSF_AT(int32_t, object, 0x0C))
                    {
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 0xA:
                    case 0xB:
                    case 0xD:
                        CScene_RemoveObjectSlot(
                            scene, nullptr,
                            reinterpret_cast<int>(
                                static_cast<uint8_t*>(entry) + 0x1C));
                        break;

                    case 0xC:
                    {
                        const int32_t subtype = CSF_AT(int32_t, entry, 0x50);
                        if (subtype != 11 && (subtype <= 21 || subtype > 23))
                            break;

                        void* specialObject = CSF_AT(void*, entry, 0x34);
                        if (!specialObject ||
                            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                                CSF_AT(void**, specialObject, 0)[0x98 / 4])(
                                    specialObject, nullptr) != 7 ||
                            !MpSpawnPoints_IsSpecialObjectActive(
                                specialObject, nullptr))
                        {
                            CScene_RemoveObjectSlot(
                                static_cast<CScene*>(g_pCurrentScene), nullptr,
                                reinterpret_cast<int>(
                                    static_cast<uint8_t*>(entry) + 0x1C));
                        }
                        else
                        {
                            MpSpawnPoints_ResetSpecialObject(
                                specialObject, nullptr);
                        }
                        break;
                    }

                    case 0xE:
                    {
                        void* spawnObject = reinterpret_cast<void*>(
                            reinterpret_cast<uintptr_t>(object) - 0x80);
                        if (spawnObject &&
                            reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                                CSF_AT(void**, object, 0)[8 / 4])(
                                    object, nullptr))
                        {
                            MpSpawnPoints_ResetSpawnObject(spawnObject, nullptr);
                        }
                        break;
                    }

                    default:
                        break;
                    }
                }
                else if (entry != CSF_AT(void*, self, 0xC8C) &&
                         reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                             CSF_AT(void**, object, 0)[8 / 4])(object, nullptr))
                {
                    CScene_RemoveObjectSlot(
                        static_cast<CScene*>(g_pCurrentScene), nullptr,
                        reinterpret_cast<int>(
                            static_cast<uint8_t*>(entry) + 0x1C));
                }
            }
        }

        if (index + 1 < objectCount)
            scene = static_cast<CScene*>(g_pCurrentScene);
    }

    void* stats = nullptr;
    if (g_CharacterSelectState8B0694)
    {
        void* statsInterface = CSF_AT(
            void*, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 4);
        if (statsInterface)
            stats = static_cast<uint8_t*>(statsInterface) - 0x308;
    }

    MpSpawnPoints_ResetStatsCategory(stats, nullptr, 1);
    MpSpawnPoints_ResetStatsCategory(stats, nullptr, 2);
    return MpSpawnPoints_RefreshStats(stats, nullptr);
}

HOOK(0x00552A20, MpSpawnPoints_ResetSceneObjects, "auto");
