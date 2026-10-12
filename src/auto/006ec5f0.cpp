#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));

static std::uint32_t* __cdecl GfxCollision_RecordPreferredIntersection(
    int /*context*/, std::uint32_t* intersection, float distance, void* query)
{
    void* object = CSF_AT(void*, query, 0x28);
    void* geometry = CSF_AT(void*, object, 0x18);
    auto* faceData = CSF_AT(std::uint8_t*, geometry, 0x2C);
    const std::uint8_t sectorIndex = faceData[intersection[6] * 8 + 6];
    auto* sectorObjects = CSF_AT(int*, geometry, 0x20);
    const int objectIndex = sectorObjects[sectorIndex];

    if (!objectIndex)
        return intersection;

    void* sector = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectSectorData(objectIndex));
    if (!sector)
        return intersection;

    std::uint8_t preferred = 0;
    if (!CSF_AT(std::uint8_t, query, 0x09))
    {
        if (CSF_AT(std::uint8_t, query, 0x08) &&
            CSF_AT(std::uint8_t, sector, 0x10))
        {
            return intersection;
        }

        preferred = (CSF_AT(std::uint32_t, query, 0x04) &
                     CSF_AT(std::uint32_t, sector, 0x0C)) != 0;
    }

    const std::uint32_t sectorValue = CSF_AT(std::uint32_t, sector, 0x08);

    if (CSF_AT(std::uint8_t, query, 0x00))
    {
        if (CSF_AT(std::uint8_t, query, 0x10) == preferred)
        {
            if (!(CSF_AT(float, query, 0x0C) > distance))
                return intersection;
        }
        else if (!preferred)
        {
            return intersection;
        }
    }

    CSF_AT(std::uint8_t, query, 0x00) = 1;
    CSF_AT(std::uint8_t, query, 0x10) = preferred;
    CSF_AT(float, query, 0x0C) = distance;
    CSF_AT(std::uint32_t, query, 0x20) = sectorValue;

    void* globalInterface = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalInterface, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, sceneController, 0)[0x44 / 4])(
            sceneController, nullptr);

    CSF_AT(std::uint8_t, query, 0x24) = 0xFF;
    CSF_AT(std::uint32_t, query, 0x14) = intersection[0];
    CSF_AT(std::uint32_t, query, 0x18) = intersection[1];
    CSF_AT(std::uint32_t, query, 0x1C) = intersection[2];

    return intersection;
}

HOOK(0x006EC5F0, GfxCollision_RecordPreferredIntersection, "auto");
