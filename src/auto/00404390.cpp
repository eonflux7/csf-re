#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using IntersectionCallback = void* (__cdecl*)(void*, void*, float, void*);

CSF_FUNC(bool, __cdecl, IsEntityExcludedOrIgnorable, 0x00402200, (S_00492b20* self, uint32_t* other));
CSF_FUNC(S_00402350*, __cdecl, Bounds_BuildFromShape, 0x004025A0, (S_00402350* result, float* shape));
CSF_FUNC(char, __cdecl, Collision_TestScaledBoxTriangles, 0x00403020, (S_006abb80* box, void* query));
CSF_FUNC(unsigned int, __fastcall, WorldCollision_ReserveCandidates, 0x00404140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, WorldCollision_ForAllAtomicIntersections, 0x00404320, (int world, int* bounds, int userParam, int owner));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, RpAtomicForAllIntersections, 0x0056BDF0, (int atomic, uint32_t* intersection, IntersectionCallback callback, int data));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(bool, __fastcall, WorldCollision_TestBoundsOverlap, 0x006AB740, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __fastcall, WorldCollision_GetBoundingSphere, 0x006AB7A0, (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* __cdecl WorldCollision_TestSceneIntersections(
    int boundsAddress,
    int* firstVector,
    int* secondVector,
    uint32_t* thirdVector,
    uint32_t* owner,
    int resultAddress)
{
    void* result = reinterpret_cast<void*>(resultAddress);
    void* inputBounds = reinterpret_cast<void*>(boundsAddress);

    alignas(8) uint8_t candidates[0x14];
    CSF_AT(void*, candidates, 0x4) = nullptr;
    CSF_AT(uint32_t*, candidates, 0x8) = nullptr;
    CSF_AT(uint32_t, candidates, 0xC) = 0;

    alignas(8) uint8_t temporaryBoxBytes[0x3C];
    alignas(8) uint8_t boxBytes[0x3C];
    S_006abb80& box = *reinterpret_cast<S_006abb80*>(boxBytes);
    alignas(8) uint8_t intersectionBytes[0x1C]{};
    auto* intersection = reinterpret_cast<uint32_t*>(intersectionBytes);
    alignas(8) uint8_t queryBytes[0x34]{};
    auto* query = reinterpret_cast<uint32_t*>(queryBytes);
    alignas(8) uint8_t boundsBytes[0x30];
    alignas(8) uint8_t sphereBytes[0x10];

    if (!CSF_AT(void*, result, 0x80))
    {
        WorldCollision_ReserveCandidates(candidates, nullptr, 5);

        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* worldObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);

        if (CSF_AT(uint8_t, worldObject, 0x91) &&
            GetReferencedObject(worldObject, nullptr))
        {
            void* world = GetReferencedObject(worldObject, nullptr);
            WorldCollision_ForAllAtomicIntersections(
                reinterpret_cast<int>(world), reinterpret_cast<int*>(boundsAddress),
                reinterpret_cast<int>(candidates), reinterpret_cast<int>(owner));
        }

        controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        CSF_AT(uint32_t, candidates, 0x10) =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0x88 / 4])(controller, nullptr);

        for (uint32_t index = 0; index < CSF_AT(uint32_t, candidates, 0x10); ++index)
        {
            void* object =
                reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, controller, 0)[0x8C / 4])(controller, nullptr, index);
            if (object && GetReferencedObject(object, nullptr))
            {
                void* world = GetReferencedObject(object, nullptr);
                WorldCollision_ForAllAtomicIntersections(
                    reinterpret_cast<int>(world), reinterpret_cast<int*>(boundsAddress),
                    reinterpret_cast<int>(candidates), reinterpret_cast<int>(owner));
            }
        }
    }

    query[7] = thirdVector[0];
    query[8] = thirdVector[1];
    query[9] = thirdVector[2];
    query[1] = firstVector[0];
    query[2] = firstVector[1];
    query[3] = firstVector[2];
    query[4] = secondVector[0];
    query[5] = secondVector[1];
    query[6] = secondVector[2];
    query[0] = static_cast<uint32_t>(resultAddress);
    query[12] = 0;

    void* scene = g_pCurrentScene;
    void* handles = CSF_AT(void*, result, 0x80);
    uint32_t remaining = handles
        ? CSF_AT(uint32_t, handles, 0x8)
        : CSF_AT(uint32_t, scene, 0x28);
    uint32_t handleOffset = 12 * remaining;

    while (remaining)
    {
        handles = CSF_AT(void*, result, 0x80);
        --remaining;
        handleOffset -= 12;
        CSF_AT(uint32_t, candidates, 0x10) = remaining;

        do
        {
            S_00492b20* entity;
            if (handles)
            {
                entity = reinterpret_cast<S_00492b20*>(
                    CHandleTable_Resolve(
                        scene, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, handles, 0) + handleOffset)));
                if (!entity)
                    break;
            }
            else
            {
                entity = CSF_AT(S_00492b20**, scene, 0x20)[remaining];
                if (!entity)
                    break;

                if (CSF_AT(uint32_t, entity, 0x50) != 34)
                {
                    void* referenced =
                        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
                    uint32_t* current = CSF_AT(uint32_t*, candidates, 0x4);
                    uint32_t* end = CSF_AT(uint32_t*, candidates, 0x8);
                    while (current != end && *current != reinterpret_cast<uint32_t>(referenced))
                        ++current;
                    if (current == end)
                        break;
                }
            }

            void* referenced =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
            void* shapeObject =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, referenced, 0)[0x70 / 4])(referenced, nullptr);
            int shapeType = CSF_AT(int, shapeObject, 0x4);
            query[11] = reinterpret_cast<uint32_t>(entity);

            if (IsEntityExcludedOrIgnorable(entity, owner))
                break;

            float* shape =
                reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, shapeObject, 0)[0x34 / 4])(shapeObject, nullptr);
            int axes[6]{};
            Bounds_BuildFromShape(reinterpret_cast<S_00402350*>(boxBytes), shape);

            if (owner && (owner[92] & 0x2000) &&
                (CSF_AT(uint32_t, entity, 0x170) & 0x2000))
            {
                box.field_000c *= g_ValueScale;
                box.field_0014 *= g_ValueScale;
            }

            S_006abb80_ComputeAxisAlignedBounds(
                &box, nullptr,
                reinterpret_cast<float*>(&axes[0]), reinterpret_cast<float*>(&axes[3]),
                reinterpret_cast<float*>(&axes[1]), reinterpret_cast<float*>(&axes[4]),
                reinterpret_cast<float*>(&axes[2]), reinterpret_cast<float*>(&axes[5]));
            std::memcpy(boundsBytes, axes, 0x18);

            if (!WorldCollision_TestBoundsOverlap(
                    inputBounds, nullptr, reinterpret_cast<float*>(boundsBytes)))
                break;

            if (shapeType == 1 ||
                (owner && (owner[92] & 0x2000) &&
                 (CSF_AT(uint32_t, entity, 0x170) & 0x2000)))
            {
                Collision_TestScaledBoxTriangles(&box, query);
                if (CSF_AT(uint8_t, reinterpret_cast<void*>(query[0]), 0x3))
                    goto Cleanup;
            }
            else if (shapeType == 2)
            {
                void* shapes =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, shapeObject, 0)[0x38 / 4])(shapeObject, nullptr);
                uint32_t shapeCount = CSF_AT(uint32_t, shapes, 0x8);
                uint32_t shapeOffset = 16 * shapeCount;

                while (shapeCount)
                {
                    uint32_t nextShapeCount = shapeCount - 1;
                    shapes =
                        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, shapeObject, 0)[0x38 / 4])(shapeObject, nullptr);
                    float* childShape = CSF_AT(
                        float*, CSF_AT(void*, shapes, 0), shapeOffset - 16);
                    S_00402350* builtBox = Bounds_BuildFromShape(
                        reinterpret_cast<S_00402350*>(temporaryBoxBytes), childShape);
                    std::memcpy(boxBytes, builtBox, 0x3C);

                    S_006abb80_ComputeAxisAlignedBounds(
                        &box, nullptr,
                        reinterpret_cast<float*>(&axes[0]), reinterpret_cast<float*>(&axes[3]),
                        reinterpret_cast<float*>(&axes[1]), reinterpret_cast<float*>(&axes[4]),
                        reinterpret_cast<float*>(&axes[2]), reinterpret_cast<float*>(&axes[5]));
                    SceneMissionSetup_SetBounds(
                        boundsBytes, nullptr, axes[0], axes[3], axes[1],
                        axes[4], axes[2], axes[5]);

                    if (WorldCollision_TestBoundsOverlap(
                            inputBounds, nullptr, reinterpret_cast<float*>(boundsBytes)))
                    {
                        Collision_TestScaledBoxTriangles(&box, query);
                        if (CSF_AT(uint8_t, reinterpret_cast<void*>(query[0]), 0x3))
                            goto Cleanup;
                    }
                    shapeCount = nextShapeCount;
                    shapeOffset -= 16;
                }
            }
            else if (shapeType == 3)
            {
                std::memset(sphereBytes, 0, 0x10);
                WorldCollision_GetBoundingSphere(
                    inputBounds, nullptr, reinterpret_cast<float*>(sphereBytes));
                std::memcpy(intersectionBytes, sphereBytes, 0x10);
                intersection[6] = 3;

                uint32_t atomicIndex = 0;
                referenced =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
                uint32_t atomicCount =
                    reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, referenced, 0)[0x84 / 4])(referenced, nullptr);
                if (!atomicCount)
                    break;

                for (;;)
                {
                    referenced =
                        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
                    void* atomicEntry =
                        reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                            CSF_AT(void**, referenced, 0)[0x80 / 4])(
                                referenced, nullptr, atomicIndex);
                    int atomic = CSF_AT(int, atomicEntry, 0xC);
                    query[10] = reinterpret_cast<uint32_t>(
                        RwFrameGetLTM(CSF_AT(uint32_t, reinterpret_cast<void*>(atomic), 0x4)));
                    RpAtomicForAllIntersections(
                        atomic, intersection,
                        reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x00402E70)),
                        reinterpret_cast<int>(query));
                    if (CSF_AT(uint8_t, reinterpret_cast<void*>(query[0]), 0x3))
                        goto Cleanup;

                    ++atomicIndex;
                    referenced =
                        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
                    atomicCount =
                        reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, referenced, 0)[0x84 / 4])(referenced, nullptr);
                    if (atomicIndex >= atomicCount)
                        break;
                }
            }
        } while (false);

        remaining = CSF_AT(uint32_t, candidates, 0x10);
        if (remaining)
            scene = g_pCurrentScene;
    }

Cleanup:
    void* block = CSF_AT(void*, candidates, 0x4);
    if (block)
        return reinterpret_cast<void*>(Mem_FreeDebug(block));
    return block;
}

HOOK(0x00404390, WorldCollision_TestSceneIntersections, "auto");
