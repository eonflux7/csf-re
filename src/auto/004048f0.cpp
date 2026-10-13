#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, Collision_ScaleDirection, 0x00401000, (float* a1, float a2, float* a3));
CSF_FUNC(float*, __fastcall, IndexedTransform_ScaleVector, 0x00401030, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, Vec3_AddAssign, 0x00401FD0, (float* self, void* , float* other));
CSF_FUNC(float*, __fastcall, AiStatic_AddVectors, 0x00402000, (float* self, void* , float* outVector, float* addVector));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_BuildContactFrame, 0x004020A0, (S_00526d40* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, Geometry_ComputeTriangleResult, 0x004020F0, (float* self, void* , float* origin, float* pointA, float* pointB));
CSF_FUNC(void*, __fastcall, ResetObjectStateFields, 0x00402530, (void* self, void*));
CSF_FUNC(char, __cdecl, Collision_ForAllSceneIntersections, 0x00403660, (int* a1, int* a2, int* a3, int* a4, int a5, int* a6));
CSF_FUNC(void*, __cdecl, Collision_ForAllObjectIntersections, 0x00404390, (int* a1, int* arg4, int* arg8, uint32_t* argC, uint32_t* a2, int arg14));
CSF_FUNC(uint32_t*, __fastcall, Collision_CombineBounds, 0x006AB6C0, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(long double, __cdecl, GfxMatrix_CalculateTriangleArea, 0x006AD4A0, (float* a, float* b, float* c));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));

CSF_GLOBAL(float, g_CollisionMinimumTriangleArea, 0x0082D4C8);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static char __cdecl Collision_MoveEllipsoid(
    float* bounds,
    float* displacement,
    char queryScene,
    char queryObjects,
    char collisionFlagA,
    char collisionFlagB,
    uint32_t* objectList,
    void* result,
    int* excludedObject,
    int collisionFilter)
{
    alignas(8) uint8_t contactFrameBytes[0x2C]{};
    auto* contactFrame = reinterpret_cast<S_00526d40*>(contactFrameBytes);

    // The combined-bound result overlaps the adjacent vector scratch space.
    alignas(8) uint8_t boundsWorkspaceBytes[0x48]{};
    auto* combinedBounds = reinterpret_cast<uint32_t*>(boundsWorkspaceBytes);
    auto* scratch = reinterpret_cast<float*>(boundsWorkspaceBytes + 0xC);
    auto* startBounds = reinterpret_cast<float*>(boundsWorkspaceBytes + 0x18);
    auto* endBounds = reinterpret_cast<float*>(boundsWorkspaceBytes + 0x30);

    // Query flags, contact data, triangle, and object identifiers form one object.
    alignas(8) uint8_t contactBytes[0x90]{};
    auto* contactPoint = reinterpret_cast<float*>(contactBytes + 0xC);

    alignas(8) uint8_t sweptBoundsBytes[0x18]{};
    auto* sweptBounds = reinterpret_cast<int*>(sweptBoundsBytes);

    alignas(8) uint8_t scaledVectorBytes[0xC]{};
    auto* scaledVector = reinterpret_cast<float*>(scaledVectorBytes);
    alignas(8) uint8_t differenceBytes[0xC]{};
    auto* difference = reinterpret_cast<float*>(differenceBytes);
    alignas(8) uint8_t sumBytes[0xC]{};
    auto* sum = reinterpret_cast<float*>(sumBytes);
    alignas(8) uint8_t projectedEndBytes[0xC]{};
    auto* projectedEnd = reinterpret_cast<float*>(projectedEndBytes);
    alignas(8) uint8_t directionBytes[0xC]{};
    auto* direction = reinterpret_cast<float*>(directionBytes);
    alignas(8) uint8_t temporaryBytes[0xC]{};
    auto* temporary = reinterpret_cast<float*>(temporaryBytes);
    alignas(8) uint8_t positionBytes[0xC]{};
    auto* position = reinterpret_cast<float*>(positionBytes);
    alignas(8) uint8_t radiiBytes[0xC]{};
    auto* radii = reinterpret_cast<float*>(radiiBytes);

    position[0] = bounds[0];
    position[1] = bounds[1];
    position[2] = bounds[2];
    direction[0] = displacement[0];
    direction[1] = displacement[1];
    direction[2] = displacement[2];
    radii[0] = bounds[3];
    radii[1] = bounds[4];
    radii[2] = bounds[5];

    CSF_AT(float, result, 0x8) = position[0];
    CSF_AT(float, result, 0xC) = position[1];
    CSF_AT(float, result, 0x10) = position[2];
    CSF_AT(uint32_t, result, 0x0) = 0;
    CSF_AT(float, result, 0x14) = g_ZeroVectorX;
    CSF_AT(float, result, 0x18) = g_ZeroVectorY;
    const float zeroZ = g_ZeroVectorZ;
    CSF_AT(uint32_t, result, 0x4C) = 0;
    CSF_AT(uint32_t, result, 0x50) = 0;
    CSF_AT(uint8_t, result, 0x55) = 0;
    CSF_AT(float, result, 0x1C) = zeroZ;
    CSF_AT(uint8_t, result, 0x54) = 0xFF;
    CSF_AT(float, result, 0x58) = g_ZeroVectorX;
    CSF_AT(float, result, 0x5C) = g_ZeroVectorY;
    CSF_AT(float, result, 0x60) = g_ZeroVectorZ;

    if (std::sqrt(displacement[0] * displacement[0]
                + displacement[1] * displacement[1]
                + displacement[2] * displacement[2])
        <= g_SceneLoadMillisecondsToSeconds)
        return 0;

    for (int i = 0; i < 6; ++i)
        sweptBounds[i] = 0;

    for (int i = 0; i < 3; ++i) {
        temporary[i] = position[i] + radii[i];
        startBounds[i] = temporary[i];
    }
    for (int i = 0; i < 3; ++i) {
        temporary[i] = position[i] - radii[i];
        startBounds[i + 3] = temporary[i];
    }

    difference[0] = direction[0] + position[0];
    const float endY = direction[1] + position[1];
    const float endZ = direction[2] + position[2];
    temporary[0] = difference[0] + radii[0];
    endBounds[0] = temporary[0];
    temporary[1] = endY + radii[1];
    endBounds[1] = temporary[1];
    temporary[2] = endZ + radii[2];
    endBounds[2] = temporary[2];
    temporary[0] = difference[0] - radii[0];
    endBounds[3] = temporary[0];
    temporary[1] = endY - radii[1];
    endBounds[4] = temporary[1];
    temporary[2] = endZ - radii[2];
    endBounds[5] = temporary[2];

    uint32_t* merged = Collision_CombineBounds(
        startBounds, nullptr, combinedBounds, endBounds);
    const float inverseX = g_TransitionOne / radii[0];
    for (int i = 0; i < 6; ++i)
        sweptBounds[i] = static_cast<int>(merged[i]);
    const float inverseY = g_TransitionOne / radii[1];
    const float inverseZ = g_TransitionOne / radii[2];
    position[0] *= inverseX;
    position[1] *= inverseY;
    position[2] *= inverseZ;
    direction[0] *= inverseX;
    direction[1] *= inverseY;
    direction[2] *= inverseZ;

    if (std::sqrt(direction[2] * direction[2]
                + direction[1] * direction[1]
                + direction[0] * direction[0])
        <= g_SceneLoadMillisecondsToSeconds)
        return 0;

    ResetObjectStateFields(contactBytes, nullptr);
    CSF_AT(int, contactBytes, 0x70) = 0;
    CSF_AT(int, contactBytes, 0x74) = 0;
    CSF_AT(int, contactBytes, 0x80) = collisionFilter;
    CSF_AT(char, contactBytes, 0x3) = 0;
    CSF_AT(char, contactBytes, 0x2) = 0;
    CSF_AT(char, contactBytes, 0x0) = collisionFlagA;
    CSF_AT(char, contactBytes, 0x1) = collisionFlagB;

    if (queryScene) {
        Collision_ForAllSceneIntersections(
            sweptBounds,
            reinterpret_cast<int*>(position),
            reinterpret_cast<int*>(direction),
            reinterpret_cast<int*>(radii),
            reinterpret_cast<int>(contactBytes),
            excludedObject);
    }

    if (queryObjects && !CSF_AT(char, contactBytes, 0x3)
        && (CSF_AT(float, contactBytes, 0x4) > g_SceneLoadMillisecondsToSeconds
            || !CSF_AT(char, contactBytes, 0x2))) {
        Collision_ForAllObjectIntersections(
            sweptBounds,
            reinterpret_cast<int*>(position),
            reinterpret_cast<int*>(direction),
            reinterpret_cast<uint32_t*>(radii),
            objectList,
            reinterpret_cast<int>(contactBytes));
    }

    if (CSF_AT(char, contactBytes, 0x3)) {
        scaledVector[0] = position[0] - contactPoint[0];
        scaledVector[1] = position[1] - contactPoint[1];
        scaledVector[2] = position[2] - contactPoint[2];
        auto* normal = reinterpret_cast<float*>(S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(scaledVector), nullptr,
            reinterpret_cast<uint32_t*>(scratch)));
        const float pushDistance =
            CSF_AT(float, contactBytes, 0x4) + g_SceneLoadMillisecondsToSeconds;
        difference[0] = pushDistance * normal[0];
        difference[1] = pushDistance * normal[1];
        difference[2] = pushDistance * normal[2];
        scaledVector[0] = difference[0] + position[0];
        position[0] = scaledVector[0];
        scaledVector[1] = difference[1] + position[1];
        position[1] = scaledVector[1];
        scaledVector[2] = difference[2] + position[2];
        position[2] = scaledVector[2];

        CSF_AT(float, result, 0x8) = scaledVector[0] * radii[0];
        CSF_AT(float, result, 0xC) = position[1] * radii[1];
        CSF_AT(float, result, 0x10) = position[2] * radii[2];
        CSF_AT(float, result, 0x14) = displacement[0];
        CSF_AT(uint32_t, result, 0x18) = CSF_AT(uint32_t, displacement, 0x4);
        CSF_AT(uint32_t, result, 0x1C) = CSF_AT(uint32_t, displacement, 0x8);
        CSF_AT(int, result, 0x0) = CSF_AT(int, contactBytes, 0x70);
        CSF_AT(int, result, 0x4C) = CSF_AT(int, contactBytes, 0x74);
        CSF_AT(int, result, 0x50) = CSF_AT(int, contactBytes, 0x78);
        CSF_AT(char, result, 0x54) = CSF_AT(char, contactBytes, 0x7C);
        CSF_AT(uint32_t, result, 0x58) = CSF_AT(uint32_t, contactBytes, 0x84);
        CSF_AT(uint32_t, result, 0x5C) = CSF_AT(uint32_t, contactBytes, 0x88);
        CSF_AT(uint32_t, result, 0x60) = CSF_AT(uint32_t, contactBytes, 0x8C);
        CSF_AT(uint8_t, result, 0x55) = 1;
        CSF_AT(float, result, 0x58) *= radii[0];
        CSF_AT(float, result, 0x5C) *= radii[1];
        CSF_AT(float, result, 0x60) *= radii[2];

        for (int i = 0; i < 3; ++i) {
            temporary[i] = CSF_AT(float, contactBytes, 0x44 + 4 * i) * radii[i];
            sum[i] = CSF_AT(float, contactBytes, 0x50 + 4 * i) * radii[i];
            projectedEnd[i] = CSF_AT(float, contactBytes, 0x5C + 4 * i) * radii[i];
        }
        if (GfxMatrix_CalculateTriangleArea(temporary, sum, projectedEnd)
            > g_CollisionMinimumTriangleArea) {
            Geometry_ComputeTriangleResult(
                reinterpret_cast<float*>(static_cast<uint8_t*>(result) + 0x20),
                nullptr, temporary, sum, projectedEnd);
        }
        return 1;
    }

    if (CSF_AT(char, contactBytes, 0x2)
        && std::sqrt(direction[2] * direction[2]
                   + direction[1] * direction[1]
                   + direction[0] * direction[0])
           > CSF_AT(float, contactBytes, 0x4)) {
        if (CSF_AT(float, contactBytes, 0x4)
            > g_SceneLoadMillisecondsToSeconds) {
            auto* normalized = reinterpret_cast<float*>(S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(direction), nullptr,
                reinterpret_cast<uint32_t*>(scratch)));
            const float advance =
                CSF_AT(float, contactBytes, 0x4) - g_SceneLoadMillisecondsToSeconds;
            float* offset = Collision_ScaleDirection(difference, advance, normalized);
            Vec3_AddAssign(position, nullptr, offset);
        }

        difference[0] = position[0] - contactPoint[0];
        difference[1] = position[1] - contactPoint[1];
        difference[2] = position[2] - contactPoint[2];
        uint32_t* normal = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(difference), nullptr,
            reinterpret_cast<uint32_t*>(scratch));
        S_00526d40_BuildContactFrame(
            contactFrame, nullptr, reinterpret_cast<uint32_t*>(contactPoint), normal);
        float* projectedStart = S_00526d40_ProjectPointToPlane(
            contactFrame, nullptr, scratch, position);
        const float remaining =
            static_cast<float>(Vector_GetLength(direction, nullptr))
            - CSF_AT(float, contactBytes, 0x4);
        S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(direction), nullptr,
            reinterpret_cast<uint32_t*>(temporary));
        float* remainingOffset = IndexedTransform_ScaleVector(
            temporary, nullptr, scaledVector, remaining);
        float* destination = AiStatic_AddVectors(
            position, nullptr, sum, remainingOffset);
        S_00526d40_ProjectPointToPlane(
            contactFrame, nullptr, projectedEnd, destination);
        float* slide = BuildFacingVectorBetween(
            projectedEnd, nullptr, difference, projectedStart);
        CSF_AT(float, result, 0x14) = slide[0];
        CSF_AT(float, result, 0x18) = slide[1];
        CSF_AT(float, result, 0x1C) = slide[2];
    } else {
        CSF_AT(char, contactBytes, 0x2) = 0;
        const float lengthSquared =
            direction[2] * direction[2]
            + direction[1] * direction[1]
            + direction[0] * direction[0];
        auto* normalized = reinterpret_cast<float*>(S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(direction), nullptr,
            reinterpret_cast<uint32_t*>(scratch)));
        const float advance =
            std::sqrt(lengthSquared) - g_SceneLoadMillisecondsToSeconds;
        difference[0] = advance * normalized[0];
        difference[1] = advance * normalized[1];
        difference[2] = advance * normalized[2];
        position[0] += difference[0];
        position[1] += difference[1];
        position[2] += difference[2];
    }

    const bool hasContact = CSF_AT(char, contactBytes, 0x2) != 0;
    CSF_AT(float, result, 0x8) = position[0] * radii[0];
    CSF_AT(float, result, 0xC) = position[1] * radii[1];
    CSF_AT(float, result, 0x10) = position[2] * radii[2];
    CSF_AT(float, result, 0x14) *= radii[0];
    CSF_AT(float, result, 0x18) *= radii[1];
    CSF_AT(float, result, 0x1C) *= radii[2];

    if (hasContact) {
        CSF_AT(uint32_t, result, 0x58) = CSF_AT(uint32_t, contactBytes, 0x84);
        CSF_AT(uint32_t, result, 0x5C) = CSF_AT(uint32_t, contactBytes, 0x88);
        CSF_AT(uint32_t, result, 0x60) = CSF_AT(uint32_t, contactBytes, 0x8C);
        CSF_AT(float, result, 0x58) *= radii[0];
        CSF_AT(float, result, 0x5C) *= radii[1];
        CSF_AT(float, result, 0x60) *= radii[2];

        for (int i = 0; i < 3; ++i) {
            projectedEnd[i] = CSF_AT(float, contactBytes, 0x44 + 4 * i) * radii[i];
            sum[i] = CSF_AT(float, contactBytes, 0x50 + 4 * i) * radii[i];
            temporary[i] = CSF_AT(float, contactBytes, 0x5C + 4 * i) * radii[i];
        }
        if (GfxMatrix_CalculateTriangleArea(projectedEnd, sum, temporary)
            > g_CollisionMinimumTriangleArea) {
            Geometry_ComputeTriangleResult(
                reinterpret_cast<float*>(static_cast<uint8_t*>(result) + 0x20),
                nullptr, projectedEnd, sum, temporary);
        }
    }

    CSF_AT(int, result, 0x0) = CSF_AT(int, contactBytes, 0x70);
    CSF_AT(int, result, 0x4C) = CSF_AT(int, contactBytes, 0x74);
    const char collided = CSF_AT(char, contactBytes, 0x2);
    CSF_AT(int, result, 0x50) = CSF_AT(int, contactBytes, 0x78);
    CSF_AT(char, result, 0x54) = CSF_AT(char, contactBytes, 0x7C);
    return collided;
}

HOOK(0x004048F0, Collision_MoveEllipsoid, "auto");
