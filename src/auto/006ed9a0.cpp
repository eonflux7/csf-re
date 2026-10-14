#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Geometry_BuildPlaneFromTriangle, 0x004023F0, (S_00526d40* self, void* , float* origin, float* pointB, float* pointC));
CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(bool, __cdecl, GfxMatrix_IsPointInsideTriangle_6AD1D0, 0x006AD1D0, (float* point, float* vertexB, float* vertexC, float* vertexD));
CSF_FUNC(float*, __cdecl, GfxMatrix_FindClosestPointOnTriangleEdges, 0x006AD320, (float* result, float* point, float* vertexA, float* vertexB, float* vertexC));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, S_006b6be0_GetTriangleSurfaceFlag, 0x006B6BE0, (S_006b6be0* self, void* , uint16_t* vertexIndices, int geometry));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));
CSF_FUNC(void*, __fastcall, GfxCollision_ResetQuery, 0x006EBDE0, (void* self, void*));
CSF_FUNC(char, __fastcall, GfxCollision_AddIntersection, 0x006ED350, (void* self, void* , float* record, char terminate));

static float* __cdecl GfxCollision_CollectSphereTriangleIntersection(
    float* center, int sector, float* triangle, float distance, void* query)
{
    if (CSF_AT(uint8_t, query, 0x08))
    {
        void* intersections = CSF_AT(void*, query, 0x0C);
        uint8_t* records = CSF_AT(uint8_t*, intersections, 0x04);
        int count = CSF_AT(int, intersections, 0x0C);
        if (!(distance < CSF_AT(float, records + 104 * count - 104, 0)))
            return triangle;
    }

    void* sectorObject = reinterpret_cast<void*>(sector);
    uint8_t* triangles = CSF_AT(uint8_t*, sectorObject, 0x04);

    alignas(8) uint8_t projectionBytes[0x10];
    float* projection = reinterpret_cast<float*>(projectionBytes);
    CSF_AT(uint16_t*, projectionBytes, 0x0C) =
        reinterpret_cast<uint16_t*>(
            triangles + 8 * CSF_AT(uint32_t, triangle, 0x18));

    uint16_t materialIndex =
        CSF_AT(uint8_t, CSF_AT(uint16_t*, projectionBytes, 0x0C), 0x06);
    bool matches = false;
    char terminate = 0;

    int world = RpWorldSectorGetWorld(static_cast<unsigned int>(sector));
    void* materials = CSF_AT(void*, reinterpret_cast<void*>(world), 0x10);
    int objectIndex = CSF_AT(int, materials, 4 * materialIndex);
    int sectorDataAddress = GfxObjectProps_GetObjectSectorData(objectIndex);
    void* sectorData = reinterpret_cast<void*>(sectorDataAddress);

    if (!CSF_AT(uint8_t, query, 0x0B))
    {
        if (!sectorData ||
            (CSF_AT(uint8_t, query, 0x0A) &&
             CSF_AT(uint8_t, sectorData, 0x10)))
            return triangle;

        matches = (CSF_AT(uint32_t, sectorData, 0x0C) &
                   CSF_AT(uint32_t, query, 0x04)) != 0;
        terminate = static_cast<char>(matches);
    }

    CSF_AT(uint8_t, query, 0x08) |= static_cast<uint8_t>(matches);
    if (!matches)
        return triangle;

    alignas(8) uint8_t planeBytes[0x2C];
    S_00526d40& plane = *reinterpret_cast<S_00526d40*>(planeBytes);
    alignas(8) uint8_t vertexABytes[0x0C];
    alignas(8) uint8_t vertexBBytes[0x0C];
    alignas(8) uint8_t vertexCBytes[0x0C];
    float* vertexA = reinterpret_cast<float*>(vertexABytes);
    float* vertexB = reinterpret_cast<float*>(vertexBBytes);
    float* vertexC = reinterpret_cast<float*>(vertexCBytes);

    float* sourceVertex = CSF_AT(float*, triangle, 0x1C);
    vertexA[0] = sourceVertex[0];
    vertexA[1] = sourceVertex[1];
    vertexA[2] = sourceVertex[2];
    sourceVertex = CSF_AT(float*, triangle, 0x20);
    vertexB[0] = sourceVertex[0];
    vertexB[1] = sourceVertex[1];
    vertexB[2] = sourceVertex[2];
    sourceVertex = CSF_AT(float*, triangle, 0x24);
    vertexC[0] = sourceVertex[0];
    vertexC[1] = sourceVertex[1];
    vertexC[2] = sourceVertex[2];

    Geometry_BuildPlaneFromTriangle(
        &plane, nullptr, vertexA, vertexB, vertexC);

    alignas(8) uint8_t centerBytes[0x0C];
    float* point = reinterpret_cast<float*>(centerBytes);
    point[0] = center[0];
    point[1] = center[1];
    point[2] = center[2];
    S_00526d40_ProjectPointToPlane(&plane, nullptr, projection, point);

    alignas(8) uint8_t closestBytes[0x18];
    float* deltaAndClosest = reinterpret_cast<float*>(closestBytes);
    float closestX;
    if (GfxMatrix_IsPointInsideTriangle_6AD1D0(
            projection, vertexA, vertexB, vertexC))
    {
        closestX = projection[0];
        deltaAndClosest[4] = projection[1];
        deltaAndClosest[5] = projection[2];
    }
    else
    {
        float* closest = GfxMatrix_FindClosestPointOnTriangleEdges(
            deltaAndClosest, point, vertexA, vertexB, vertexC);
        closestX = closest[0];
        deltaAndClosest[4] = closest[1];
        deltaAndClosest[5] = closest[2];
    }

    deltaAndClosest[3] = closestX;
    deltaAndClosest[0] = point[0] - deltaAndClosest[3];
    deltaAndClosest[1] = point[1] - deltaAndClosest[4];
    deltaAndClosest[2] = point[2] - deltaAndClosest[5];

    alignas(8) uint8_t recordBytes[0x68];
    float* record = reinterpret_cast<float*>(recordBytes);
    GfxCollision_ResetQuery(recordBytes, nullptr);

    record[2] = deltaAndClosest[4];
    record[3] = deltaAndClosest[5];
    float normalX = triangle[0];
    float normalY = triangle[1];
    float normalZ = triangle[2];
    record[4] = normalX;
    record[1] = closestX;
    record[5] = normalY;
    record[6] = normalZ;
    record[0] = deltaAndClosest[2] * deltaAndClosest[2] +
                deltaAndClosest[1] * deltaAndClosest[1] +
                deltaAndClosest[0] * deltaAndClosest[0];

    CSF_AT(uint32_t, recordBytes, 0x1C) =
        sectorData ? CSF_AT(uint32_t, sectorData, 0x08) : 0xFFFFFFFFu;

    int global = GetGlobalInt();
    int controllerAddress =
        SceneLoader_GetSceneController(reinterpret_cast<void*>(global), nullptr);
    void* controller = reinterpret_cast<void*>(controllerAddress);
    S_006b6be0* surfaceInfo =
        reinterpret_cast<S_006b6be0* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x44 / 4])(controller, nullptr);

    // The packet also identifies this direct callee as a vtable reference.
    (void)CSF_VTBL(0x006B6BE0);
    CSF_AT(char, recordBytes, 0x20) =
        S_006b6be0_GetTriangleSurfaceFlag(
            surfaceInfo, nullptr,
            CSF_AT(uint16_t*, projectionBytes, 0x0C), sector);

    // 006EDBE3: mov ecx, [edi+0x0c]; 006EDBE6: call 006ED350.
    GfxCollision_AddIntersection(CSF_AT(void*, query, 0x0C), nullptr, record, terminate);
    return triangle;
}

HOOK(0x006ED9A0, GfxCollision_CollectSphereTriangleIntersection, "auto");
