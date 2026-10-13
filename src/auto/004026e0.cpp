#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, HudAlertIcons_BuildQueryContext, 0x004021C0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, Geometry_BuildPlaneFromTriangle, 0x004023F0, (S_00526d40* self, void* , float* origin, float* pointB, float* pointC));
CSF_FUNC(double, __fastcall, PlaneDistanceToPoint, 0x004024D0, (float* self, void* , float* point));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, S_00526d40_IntersectPlane, 0x006ACCD0, (S_00526d40* self, void* , int planeAddress, float* intersection));
CSF_FUNC(int, __fastcall, GfxMatrix_IntersectRaySphere_6ACF20, 0x006ACF20, (float* self, void* , float* sphere, float* firstHit, float* secondHit));
CSF_FUNC(bool, __cdecl, GfxMatrix_IsPointInsideTriangle_6AD1D0, 0x006AD1D0, (float* point, float* vertexB, float* vertexC, float* vertexD));
CSF_FUNC(float*, __cdecl, GfxMatrix_FindClosestPointOnTriangleEdges, 0x006AD320, (float* result, float* point, float* vertexA, float* vertexB, float* vertexC));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, GfxObject_ProcessDeepestLeaf, 0x006B2AF0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_CollisionHeightTolerance, 0x0082D4D0);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float Collision_ReadFloatBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static char __cdecl Collision_TestTriangle(float* triangle, float* center,
                                           S_00526d40* motion,
                                           int sourceType, int sourceAddress,
                                           int resultAddress)
{
    alignas(8) uint8_t planeBytes[0x2C];
    alignas(8) uint8_t scratchBytes[0xC];
    alignas(8) uint8_t rayBytes[0x18];
    alignas(8) uint8_t sphereBytes[0x14];
    alignas(8) uint8_t projectedBytes[0xC];
    alignas(8) uint8_t reverseDirectionBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t hitBytes[0xC];
    alignas(8) uint8_t closestBytes[0xC];

    auto* plane = reinterpret_cast<float*>(planeBytes);
    auto* scratch = reinterpret_cast<float*>(scratchBytes);
    auto* ray = reinterpret_cast<float*>(rayBytes);
    auto* sphere = reinterpret_cast<float*>(sphereBytes);
    auto* projected = reinterpret_cast<float*>(projectedBytes);
    auto* reverseDirection = reinterpret_cast<float*>(reverseDirectionBytes);
    auto* direction = reinterpret_cast<float*>(directionBytes);
    auto* hit = reinterpret_cast<float*>(hitBytes);
    auto* closest = reinterpret_cast<float*>(closestBytes);
    auto* result = reinterpret_cast<uint8_t*>(resultAddress);

    const float edgeCX = triangle[6] - triangle[0];
    const float edgeCY = triangle[7] - triangle[1];
    const float edgeCZ = triangle[8] - triangle[2];

    ray[0] = 1.0f;
    ray[1] = 0.0f;
    ray[2] = 0.0f;
    ray[3] = 0.0f;
    ray[4] = 0.0f;
    ray[5] = 0.0f;
    projected[0] = 0.0f;
    projected[1] = 0.0f;
    projected[2] = 0.0f;

    hit[0] = triangle[3] - triangle[0];
    hit[1] = triangle[4] - triangle[1];
    const float edgeBZ = triangle[5] - triangle[2];
    direction[0] = hit[1] * edgeCZ - edgeBZ * edgeCY;
    direction[1] = edgeBZ * edgeCX - edgeCZ * hit[0];
    const float normalZ = edgeCY * hit[0] - hit[1] * edgeCX;

    if (g_fZero == normalZ * normalZ
                    + direction[1] * direction[1]
                    + direction[0] * direction[0])
        return 1;

    Geometry_BuildPlaneFromTriangle(
        reinterpret_cast<S_00526d40*>(planeBytes), nullptr,
        triangle, triangle + 3, triangle + 6);

    if (CSF_AT(uint8_t, result, 0x1)
        && plane[4] * Collision_ReadFloatBits(motion[1].field_0000)
             + plane[3] * Collision_ReadFloatBits(motion->field_0000)
             + plane[5] * Collision_ReadFloatBits(motion[2].field_0000)
               > g_fZero)
        return 1;

    S_00526d40_ConvertDirection(
        motion, nullptr, reinterpret_cast<uint32_t*>(directionBytes));

    if (CSF_AT(uint8_t, result, 0x1)) {
        closest[0] = direction[0] + center[0];
        closest[1] = direction[1] + center[1];
        closest[2] = direction[2] + center[2];
    } else {
        closest[0] = center[0] - plane[3];
        closest[1] = center[1] - plane[4];
        closest[2] = center[2] - plane[5];
    }
    hit[0] = closest[0];
    hit[1] = closest[1];
    hit[2] = closest[2];

    if (!(PlaneDistanceToPoint(plane, nullptr, center) > g_TransitionOne)) {
        float* point = S_00526d40_ProjectPointToPlane(
            reinterpret_cast<S_00526d40*>(planeBytes), nullptr,
            scratch, center);
        projected[0] = point[0];
        projected[1] = point[1];
        projected[2] = point[2];
    } else {
        ray[0] = direction[0];
        ray[1] = direction[1];
        ray[2] = direction[2];
        ray[3] = hit[0];
        ray[4] = hit[1];
        ray[5] = hit[2];
        NormalizeVector3(reinterpret_cast<C_00833b88*>(rayBytes), nullptr);
        if (!S_00526d40_IntersectPlane(
                reinterpret_cast<S_00526d40*>(rayBytes), nullptr,
                reinterpret_cast<int>(planeBytes), projected))
            return 1;
    }

    if (GfxMatrix_IsPointInsideTriangle_6AD1D0(
            projected, triangle, triangle + 3, triangle + 6)) {
        closest[0] = projected[0];
        closest[1] = projected[1];
        closest[2] = projected[2];
    } else {
        float* point = GfxMatrix_FindClosestPointOnTriangleEdges(
            scratch, center, triangle, triangle + 3, triangle + 6);
        closest[0] = point[0];
        closest[1] = point[1];
        closest[2] = point[2];
    }

    const float deltaY = closest[1] - center[1];
    const float deltaZ = closest[2] - center[2];
    const float deltaX = closest[0] - center[0];
    const double distance = std::sqrt(
        static_cast<double>(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX));

    if (distance >= g_TransitionOne) {
        sphere[0] = center[0];
        sphere[1] = center[1];
        sphere[2] = center[2];
        sphere[3] = 1.0f;
        reverseDirection[0] = -direction[0];
        reverseDirection[1] = -direction[1];
        reverseDirection[2] = -direction[2];
        hit[0] = 0.0f;
        hit[1] = 0.0f;
        hit[2] = 0.0f;
        std::memset(scratchBytes, 0, sizeof(scratchBytes));

        HudAlertIcons_BuildQueryContext(
            reinterpret_cast<C_00833b88*>(rayBytes), nullptr,
            reinterpret_cast<uint32_t*>(closestBytes),
            reinterpret_cast<uint32_t*>(reverseDirectionBytes));
        if (!GfxMatrix_IntersectRaySphere_6ACF20(
                ray, nullptr, sphere, hit, scratch))
            return 1;

        double travel;
        if (CSF_AT(uint8_t, result, 0x0)
            && Collision_ReadFloatBits(motion[1].field_0000) == g_fZero
            && std::fabs(center[1] - closest[1]) <= g_CollisionHeightTolerance) {
            sphere[0] = center[0];
            sphere[1] = closest[1];
            sphere[2] = center[2];
            reverseDirection[0] = sphere[0] - closest[0];
            reverseDirection[1] = closest[1] - closest[1];
            reverseDirection[2] = sphere[2] - closest[2];

            if (Vector_GetLength(reverseDirection, nullptr) >= g_TransitionOne) {
                GfxMatrix_IntersectRaySphere_6ACF20(
                    ray, nullptr, sphere, hit, scratch);
                reverseDirection[0] = hit[0] - closest[0];
                reverseDirection[1] = hit[1] - closest[1];
                reverseDirection[2] = hit[2] - closest[2];
                travel = static_cast<double>(
                    Vector_GetLength(reverseDirection, nullptr));
            } else {
                travel = 0.0;
            }
        } else {
            const float x = hit[0] - closest[0];
            const float z = hit[2] - closest[2];
            const float y = hit[1] - closest[1];
            travel = std::sqrt(static_cast<double>(x * x + z * z + y * y));
        }

        const uint8_t hasHit = CSF_AT(uint8_t, result, 0x2);
        sphere[4] = plane[4] * direction[1]
                      + plane[3] * direction[0]
                      + plane[5] * direction[2];

        if (!hasHit
            || travel < CSF_AT(float, result, 0x4)
            || (travel == CSF_AT(float, result, 0x4)
                && sphere[4] < CSF_AT(float, result, 0x8))) {
            CSF_AT(float, result, 0x4) = static_cast<float>(travel);
            CSF_AT(float, result, 0xC) = closest[0];
            CSF_AT(float, result, 0x10) = closest[1];
            CSF_AT(float, result, 0x14) = closest[2];
            std::memcpy(result + 0x18, planeBytes, 0x2C);
            CSF_AT(float, result, 0x8) = sphere[4];
            CSF_AT(uint8_t, result, 0x2) = 1;
            std::memcpy(result + 0x44, triangle, 0x2C);
            CSF_AT(float, result, 0x84) = closest[0];
            CSF_AT(float, result, 0x88) = closest[1];
            CSF_AT(int, result, 0x70) = sourceType;
            CSF_AT(int, result, 0x74) = sourceAddress;
            CSF_AT(float, result, 0x8C) = closest[2];

            if (sourceType == 1) {
                CSF_AT(float, result, 0x78) = triangle[9];
                CSF_AT(uint8_t, result, 0x7C) = CSF_AT(uint8_t, triangle, 0x28);
            } else {
                CSF_AT(int, result, 0x78) = -1;
                void* source = reinterpret_cast<void*>(sourceAddress);
                const int leaf =
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, source, 0)[40 / 4])(source, nullptr);
                CSF_AT(uint8_t, result, 0x7C) = static_cast<uint8_t>(
                    GfxObject_ProcessDeepestLeaf(reinterpret_cast<void*>(leaf), nullptr));
            }
        }
        return 1;
    }

    CSF_AT(uint8_t, result, 0x3) = 1;
    const float overlapX = closest[0] - center[0];
    const float overlapY = closest[1] - center[1];
    const float overlapZ = closest[2] - center[2];
    CSF_AT(float, result, 0xC) = closest[0];
    CSF_AT(float, result, 0x10) = closest[1];
    CSF_AT(float, result, 0x14) = closest[2];
    std::memcpy(result + 0x18, planeBytes, 0x2C);
    CSF_AT(float, result, 0x4) = static_cast<float>(
        1.0 - std::sqrt(static_cast<double>(
            overlapZ * overlapZ + overlapY * overlapY + overlapX * overlapX)));
    std::memcpy(result + 0x44, triangle, 0x2C);
    CSF_AT(float, result, 0x84) = closest[0];
    CSF_AT(float, result, 0x88) = closest[1];
    CSF_AT(int, result, 0x70) = sourceType;
    CSF_AT(int, result, 0x74) = sourceAddress;
    CSF_AT(float, result, 0x8C) = closest[2];

    if (sourceType == 1) {
        CSF_AT(float, result, 0x78) = triangle[9];
        CSF_AT(uint8_t, result, 0x7C) = CSF_AT(uint8_t, triangle, 0x28);
    } else {
        CSF_AT(int, result, 0x78) = -1;
        void* source = reinterpret_cast<void*>(sourceAddress);
        const int leaf =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[40 / 4])(source, nullptr);
        CSF_AT(uint8_t, result, 0x7C) = static_cast<uint8_t>(
            GfxObject_ProcessDeepestLeaf(reinterpret_cast<void*>(leaf), nullptr));
    }
    return 0;
}

HOOK(0x004026E0, Collision_TestTriangle, "auto");
