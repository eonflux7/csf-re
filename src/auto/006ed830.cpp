#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxCollision_GetTriangleSurfaceFlag, 0x006B6BE0, (void* self, void* edx, uint16_t* a2, int a3));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));
CSF_FUNC(char, __fastcall, GfxCollision_InsertIntersection, 0x006ED350, (void* self, void* edx, float* a2, char a3));

[[maybe_unused]] static const auto kTriangleSurfaceVtable = CSF_VTBL(0x006B6BE0);

static int __cdecl GfxCollision_CollectSectorIntersection(
    int /*intersection*/, int sector, int hit, float distance, int context)
{
    void* query = reinterpret_cast<void*>(context);
    void* sectorData = reinterpret_cast<void*>(sector);
    void* hitData = reinterpret_cast<void*>(hit);

    if (CSF_AT(uint8_t, query, 0x8)) {
        void* results = CSF_AT(void*, query, 0xC);
        const int count = CSF_AT(int, results, 0xC);
        uint8_t* entries = CSF_AT(uint8_t*, results, 0x4);
        if (!(distance < CSF_AT(float, entries + 104 * count - 104, 0x0)))
            return hit;
    }

    uint16_t* triangle = reinterpret_cast<uint16_t*>(
        CSF_AT(uint8_t*, sectorData, 0x4) +
        8 * CSF_AT(int, hitData, 0x18));
    const uint16_t objectIndex = CSF_AT(uint8_t, triangle, 0x6);

    const int world = RpWorldSectorGetWorld(static_cast<unsigned int>(sector));
    int* objects = CSF_AT(int*, reinterpret_cast<void*>(world), 0x10);
    void* properties = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectSectorData(objects[objectIndex]));

    char matchesMask = 0;
    if (!CSF_AT(uint8_t, query, 0xB)) {
        if (!properties ||
            (CSF_AT(uint8_t, query, 0xA) &&
             CSF_AT(uint8_t, properties, 0x10)))
            return hit;

        matchesMask = (CSF_AT(uint32_t, properties, 0xC) &
                       CSF_AT(uint32_t, query, 0x4)) != 0;
    }

    CSF_AT(uint8_t, query, 0x8) |= static_cast<uint8_t>(matchesMask);
    if (!matchesMask && CSF_AT(uint8_t, query, 0x9))
        return hit;

    alignas(8) uint8_t record[0x6C] = {};
    CSF_AT(float, record, 0x0) = distance;
    CSF_AT(float, record, 0x10) = CSF_AT(float, hitData, 0x0);
    const float hitY = CSF_AT(float, hitData, 0x4);
    const float hitZ = CSF_AT(float, hitData, 0x8);
    std::memset(record + 0x4, 0, 12);
    CSF_AT(int, record, 0x64) = 10;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(float, record, 0x14) = hitY;
    CSF_AT(float, record, 0x18) = hitZ;
    CSF_AT(int, record, 0x1C) =
        properties ? CSF_AT(int, properties, 0x8) : -1;

    const int globals = GetGlobalInt();
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(globals), nullptr));
    void* surfaceData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0x0)[0x44 / 4])(controller, nullptr);

    CSF_AT(char, record, 0x20) =
        GfxCollision_GetTriangleSurfaceFlag(surfaceData, nullptr, triangle, sector);

    CSF_AT(int, record, 0x5C) =
        RpWorldSectorGetWorld(static_cast<unsigned int>(sector));
    CSF_AT(int, record, 0x60) = 0;
    std::memcpy(record + 0x34, hitData, 40);
    CSF_AT(char, record, 0x28) = matchesMask;
    CSF_AT(int, record, 0x2C) = CSF_AT(int, record, 0x1C);
    CSF_AT(char, record, 0x30) = CSF_AT(char, record, 0x20);

    GfxCollision_InsertIntersection(
        CSF_AT(void*, query, 0xC), nullptr,
        reinterpret_cast<float*>(record), matchesMask);
    return hit;
}

HOOK(0x006ED830, GfxCollision_CollectSectorIntersection, "auto");
