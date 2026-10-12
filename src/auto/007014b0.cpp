#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_ParticleNearDepthMultiplier, 0x00863174);

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_ClassifyPointAgainstPlane, 0x006AD7B0, (float* self, void* , float* point));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_00867358_ClearFields, 0x006C3620, (C_00867358* self, void*));
CSF_FUNC(int, __fastcall, GfxParticleSystemDefs_AppendTriangleVertex, 0x006C36A0,
         (void* self, void* edx, uint32_t* a2, uint8_t* a3, uint32_t* a4));
CSF_FUNC(int, __fastcall, GfxParticleSystemDefs_RegisterBatchInstance, 0x007012E0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxParticleSystemDefs_FindBatchInstance, 0x00701470,
         (void* self, void* edx, int a2, int a3));

// The packet's CSF_VTBL(0x0041E5F0) entry describes the direct
// GrowArrayCapacity call at 0x00701780, not a vtable assignment.
static float* __cdecl GfxParticleSystemDefs_CollectSectorTriangle(
    int /*a1*/, int sectorAddress, int triangleAddress,
    int /*a4*/, int contextAddress)
{
    auto* sector = reinterpret_cast<uint8_t*>(sectorAddress);
    auto* triangle = reinterpret_cast<float*>(triangleAddress);

    const uint16_t materialIndex = CSF_AT(
        uint8_t, CSF_AT(uint8_t*, sector, 0x04),
        8 * CSF_AT(int, triangle, 0x18) + 6);

    const int world = RpWorldSectorGetWorld(
        static_cast<unsigned int>(sectorAddress));
    const int sectorData = GfxObjectProps_GetObjectSectorData(
        CSF_AT(int, CSF_AT(void*, reinterpret_cast<void*>(world), 0x10),
               4 * materialIndex));

    bool acceptsTriangle = true;
    if (sectorData)
        acceptsTriangle =
            (CSF_AT(uint32_t, reinterpret_cast<void*>(sectorData), 0x0C)
             & 0x80) != 0;

    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    if (CSF_AT(void*, context, 0x28) && acceptsTriangle)
    {
        const float normalDot =
            triangle[1] * CSF_AT(float, context, 0x0C)
            + CSF_AT(float, context, 0x08) * triangle[0]
            + triangle[2] * CSF_AT(float, context, 0x10);

        if (!(-CSF_AT(float, CSF_AT(void*, context, 0x00), 0x04)
              < normalDot))
        {
            // Plane occupies the beginning; transformed vertices start at +0x2C.
            alignas(8) uint8_t planeAndVertices[0x50];
            alignas(8) uint8_t sourceVertexBytes[0x14];
            uint8_t intensities[3];
            int nearVertexCount = 0;
            int farVertexCount = 0;

            for (int i = 0; i < 3; ++i)
            {
                auto* transformed = planeAndVertices + 0x2C + 12 * i;
                RwV3dTransformPoints(
                    reinterpret_cast<int>(transformed),
                    CSF_AT(int, triangle, 0x1C + 4 * i),
                    1, CSF_AT(int, context, 0x04));

                const float farDepth = CSF_AT(float, context, 0x1C);
                if (CSF_AT(float, context, 0x14)
                    * g_ParticleNearDepthMultiplier
                    > CSF_AT(float, transformed, 0x08))
                    ++nearVertexCount;

                if (CSF_AT(float, transformed, 0x08)
                    > g_ScoreboardRefreshInterval * farDepth)
                    ++farVertexCount;

                float fade =
                    (CSF_AT(float, transformed, 0x08)
                     - (CSF_AT(float, context, 0x14)
                        + CSF_AT(float, context, 0x14)))
                    / (CSF_AT(float, context, 0x18)
                       * g_ScoreboardRefreshInterval);

                if (fade > g_TransitionOne)
                    fade = g_TransitionOne;
                else if (fade < g_fZero)
                    fade = g_fZero;

                intensities[i] = static_cast<uint8_t>(
                    static_cast<int64_t>(
                        (g_TransitionOne - fade)
                        * CSF_AT(float, context, 0x20)));
            }

            if (nearVertexCount < 3
                && farVertexCount < 3
                && (!(CSF_AT(float, planeAndVertices, 0x2C) > g_TransitionOne)
                    || !(CSF_AT(float, planeAndVertices, 0x38) > g_TransitionOne)
                    || !(CSF_AT(float, planeAndVertices, 0x44) > g_TransitionOne))
                && (!(CSF_AT(float, planeAndVertices, 0x2C) < g_fZero)
                    || !(CSF_AT(float, planeAndVertices, 0x38) < g_fZero)
                    || !(CSF_AT(float, planeAndVertices, 0x44) < g_fZero))
                && (!(CSF_AT(float, planeAndVertices, 0x30) > g_TransitionOne)
                    || !(CSF_AT(float, planeAndVertices, 0x3C) > g_TransitionOne)
                    || !(CSF_AT(float, planeAndVertices, 0x48) > g_TransitionOne))
                && (!(CSF_AT(float, planeAndVertices, 0x30) < g_fZero)
                    || !(CSF_AT(float, planeAndVertices, 0x3C) < g_fZero)
                    || !(CSF_AT(float, planeAndVertices, 0x48) < g_fZero)))
            {
                auto* firstVertex = CSF_AT(uint32_t*, triangle, 0x1C);
                const uint32_t x = firstVertex[0];
                const uint32_t y = firstVertex[1];
                const uint32_t z = firstVertex[2];

                CSF_AT(uint32_t, sourceVertexBytes, 0x00) = x;
                CSF_AT(uint32_t, sourceVertexBytes, 0x04) = y;
                CSF_AT(uint32_t, sourceVertexBytes, 0x08) = z;
                CSF_AT(uint32_t, planeAndVertices, 0x00) = x;
                CSF_AT(uint32_t, planeAndVertices, 0x04) = y;
                CSF_AT(uint32_t, planeAndVertices, 0x08) = z;

                CSF_AT(uint32_t, planeAndVertices, 0x0C) =
                    CSF_AT(uint32_t, triangle, 0x00);
                CSF_AT(uint32_t, planeAndVertices, 0x10) =
                    CSF_AT(uint32_t, triangle, 0x04);
                CSF_AT(uint32_t, planeAndVertices, 0x14) =
                    CSF_AT(uint32_t, triangle, 0x08);
                CSF_AT(uint8_t, planeAndVertices, 0x28) = 1;

                const int firstSide = GfxMatrix_ClassifyPointAgainstPlane(
                    reinterpret_cast<float*>(planeAndVertices), nullptr,
                    reinterpret_cast<float*>(context + 0x3C));

                if (GfxMatrix_ClassifyPointAgainstPlane(
                        reinterpret_cast<float*>(planeAndVertices), nullptr,
                        reinterpret_cast<float*>(context + 0x48)) == firstSide)
                {
                    if (CSF_AT(int, CSF_AT(void*, context, 0x28), 0x0C) + 3
                        > CSF_AT(int, CSF_AT(void*, context, 0x28), 0x08))
                    {
                        const int instanceIndex =
                            GfxParticleSystemDefs_RegisterBatchInstance(
                                CSF_AT(void*, context, 0x2C), nullptr,
                                CSF_AT(int, context, 0x00));

                        if (instanceIndex == -1)
                        {
                            CSF_AT(void*, context, 0x28) = nullptr;
                        }
                        else
                        {
                            auto* indices = reinterpret_cast<S_0041e5f0*>(
                                CSF_AT(uint8_t*, context, 0x00) + 0x0C);

                            if (CSF_AT(int, CSF_AT(void*, context, 0x00), 0x14)
                                >= CSF_AT(int, CSF_AT(void*, context, 0x00), 0x10))
                            {
                                S_0041e5f0_GrowArrayCapacity(
                                    indices, nullptr,
                                    CSF_AT(int, CSF_AT(void*, context, 0x00), 0x18));
                            }

                            const uint32_t count = indices->field_0008;
                            auto* destination = reinterpret_cast<int*>(
                                indices->field_0000 + 4 * count);
                            indices->field_0008 = count + 1;
                            *destination = instanceIndex;

                            auto* instance = reinterpret_cast<C_00867358*>(
                                GfxParticleSystemDefs_FindBatchInstance(
                                    CSF_AT(void*, context, 0x2C), nullptr,
                                    instanceIndex, CSF_AT(int, context, 0x00)));
                            CSF_AT(C_00867358*, context, 0x28) = instance;
                            C_00867358_ClearFields(instance, nullptr);
                        }
                    }

                    void* instance = CSF_AT(void*, context, 0x28);
                    if (instance)
                    {
                        for (int i = 0; i < 3; ++i)
                        {
                            uint32_t color;
                            if (CSF_AT(uint8_t, CSF_AT(void*, context, 0x00), 0x08))
                            {
                                color = static_cast<uint32_t>(intensities[i]) << 24;
                            }
                            else
                            {
                                const uint32_t intensity = intensities[i];
                                color = intensity | (intensity << 8)
                                        | (intensity << 16);
                            }

                            auto* vertex =
                                CSF_AT(uint32_t*, triangle, 0x1C + 4 * i);
                            CSF_AT(uint32_t, sourceVertexBytes, 0x00) = vertex[0];
                            CSF_AT(uint32_t, sourceVertexBytes, 0x04) = vertex[1];
                            CSF_AT(uint32_t, sourceVertexBytes, 0x08) = vertex[2];

                            GfxParticleSystemDefs_AppendTriangleVertex(
                                instance, nullptr,
                                reinterpret_cast<uint32_t*>(sourceVertexBytes),
                                reinterpret_cast<uint8_t*>(&color),
                                reinterpret_cast<uint32_t*>(
                                    planeAndVertices + 0x2C + 12 * i));
                        }
                    }
                }
            }
        }
    }

    return CSF_AT(void*, context, 0x28) ? triangle : nullptr;
}

HOOK(0x007014B0, GfxParticleSystemDefs_CollectSectorTriangle, "auto");
