#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwBBoxContainsPoint, 0x0063CA30, (float* a1, float* a2));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyVec3AndUpdate, 0x006AD950, (uint32_t* self, void* , uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetTranslationAndUpdate, 0x006AD980, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, C_006D7420_GetSubObjectAt0x108, 0x006D7420, (void* self, void*));
CSF_FUNC(char, __fastcall, C_6d7430_GetByte118, 0x006D7430, (void* self, void*));
CSF_FUNC(char, __fastcall, GfxParticleRender_TestBoundaryFace, 0x006D7450, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(char, __fastcall, GfxParticleRender_TestSweptBoundary, 0x006D7720, (void* self, void* edx, float a2, float* a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(float, g_ParticleVectorLengthThreshold, 0x0086319C);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __cdecl GfxParticleRender_UpdateParticleBatch(
    int emitterAddress, int batchAddress, int contextAddress)
{
    auto* emitter = reinterpret_cast<std::uint8_t*>(emitterAddress);
    auto* batch = reinterpret_cast<std::uint8_t*>(batchAddress);
    auto* context = reinterpret_cast<std::uint8_t*>(contextAddress);

    void* contextEmitter = CSF_AT(void*, context, 0x4);
    void* propertyLink = CSF_AT(void*, contextEmitter, 0x1C);
    void* settings = CSF_AT(void*, propertyLink, 0x0);
    auto* matrixStream = CSF_AT(std::uint32_t*, context, 0x8);
    auto* colorStream = CSF_AT(std::uint32_t*, context, 0xC);
    const bool hasMatrices = CSF_AT(std::uint8_t, context, 0x10) != 0;

    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* boundaryController =
        reinterpret_cast<void*>(
            (reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, registry, 0)[0x44 / 4]))(registry, nullptr));

    const float horizontalX = CSF_AT(float, settings, 0x34);
    const float horizontalY = CSF_AT(float, settings, 0x38);
    const bool sweptTest =
        horizontalX * horizontalX + horizontalY * horizontalY > g_Const200_0f;
    const bool orientParticles =
        CSF_AT(std::uint32_t, settings, 0x100) != 0 &&
        CSF_AT(std::uint8_t, settings, 0x104) != 0;
    const bool applyDrag = CSF_AT(float, settings, 0x84) != g_fZero;
    const bool testBoundaries =
        boundaryController &&
        C_6d7430_GetByte118(boundaryController, nullptr) != 0;

    if (!orientParticles && !applyDrag && !testBoundaries)
        return batchAddress;

    void* particleClass = CSF_AT(void*, emitter, 0x18);
    const int particleStride = CSF_AT(int, particleClass, 0xC);
    const int propertyTable = CSF_AT(int, particleClass, 0x10);
    const int particleCount = CSF_AT(int, batch, 0x8);
    auto* particleData = batch + CSF_AT(int, batch, 0x14);

    const int velocityOffset = RpPrtStdPropTabGetPropOffset(propertyTable, 6);
    const int auxiliaryOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0x18), 0x10), 5);

    const int ageRelativeOffset = 4 - velocityOffset;
    const int lifetimeRelativeOffset = 8 - velocityOffset;
    const int auxiliaryRelativeOffset = auxiliaryOffset - velocityOffset;
    auto* velocityBytes = particleData + velocityOffset;

    for (int remaining = particleCount; remaining > 0; --remaining)
    {
        auto* velocity = reinterpret_cast<float*>(velocityBytes);
        auto* auxiliary = auxiliaryOffset == -1
            ? nullptr
            : reinterpret_cast<float*>(velocityBytes + auxiliaryRelativeOffset);

        const float timeStep =
            *reinterpret_cast<float*>(velocityBytes + ageRelativeOffset) > g_fZero
                ? CSF_AT(float, context, 0x0)
                : 0.0f;

        auto* matrix = hasMatrices
            ? reinterpret_cast<float*>(matrixStream[0])
            : nullptr;
        const std::uint32_t scalarLocation = hasMatrices ? 0 : matrixStream[0];

        if (orientParticles)
        {
            const double length = RwV3dLength(velocity);
            const float savedLength = static_cast<float>(length);
            if (length > g_ParticleVectorLengthThreshold)
            {
                alignas(8) std::uint8_t translationBytes[0x14];
                alignas(8) std::uint8_t tangentBytes[0x20];
                alignas(8) std::uint8_t matrixReferenceBytes[0x14];

                auto* translation = reinterpret_cast<float*>(translationBytes);
                auto* tangent = reinterpret_cast<float*>(tangentBytes);
                translation[0] = velocity[0];
                translation[1] = velocity[1];
                translation[2] = velocity[2];

                CSF_AT(float*, matrixReferenceBytes, 0x0) = matrix;
                CSF_AT(std::uint8_t, matrixReferenceBytes, 0x4) = 0;

                tangent[0] = translation[2] * matrix[9] - translation[1] * matrix[10];
                tangent[1] = translation[0] * matrix[10] - translation[2] * matrix[8];
                tangent[2] = translation[1] * matrix[8] - translation[0] * matrix[9];
                NormalizeVector3(
                    reinterpret_cast<C_00833b88*>(tangentBytes), nullptr);

                if (auxiliary)
                {
                    const float tangentScale = auxiliary[4];
                    tangent[0] *= tangentScale;
                    tangent[1] *= tangentScale;
                    tangent[2] *= tangentScale;

                    const float radialAmount = auxiliary[5];
                    const float configuredScale = CSF_AT(float, settings, 0x108);
                    const float radialScale =
                        (configuredScale == g_fZero || std::isnan(configuredScale))
                            ? radialAmount / savedLength
                            : radialAmount * CSF_AT(float, settings, 0x108);
                    translation[0] *= radialScale;
                    translation[1] *= radialScale;
                    translation[2] *= radialScale;
                }

                GfxMatrix_SetTranslationAndUpdate(
                    matrixReferenceBytes, nullptr,
                    reinterpret_cast<const std::uint32_t*>(translationBytes));
                GfxMatrix_CopyVec3AndUpdate(
                    reinterpret_cast<std::uint32_t*>(matrixReferenceBytes), nullptr,
                    reinterpret_cast<std::uint32_t*>(tangentBytes));

                if (CSF_AT(std::uint8_t, matrixReferenceBytes, 0x4))
                    RwMatrixDestroy(
                        CSF_AT(std::uint32_t*, matrixReferenceBytes, 0x0));
            }
        }

        if (applyDrag)
        {
            const double length = RwV3dLength(velocity);
            const double drag = -(length * timeStep * CSF_AT(float, settings, 0x84));
            if (std::fabs(drag) >= g_TransitionOne)
            {
                velocity[0] = g_ZeroVectorX;
                velocity[1] = g_ZeroVectorY;
                velocity[2] = g_ZeroVectorZ;
            }
            else
            {
                const double deltaX = drag * velocity[0];
                const double deltaY = drag * velocity[1];
                const float deltaZ = static_cast<float>(drag * velocity[2]);
                velocity[0] = static_cast<float>(deltaX + velocity[0]);
                velocity[1] = static_cast<float>(deltaY + velocity[1]);
                velocity[2] = deltaZ + velocity[2];
            }
        }

        if (testBoundaries)
        {
            void* boundaries =
                C_006D7420_GetSubObjectAt0x108(boundaryController, nullptr);
            const std::uint32_t locationBits = hasMatrices
                ? reinterpret_cast<std::uint32_t>(matrix + 12)
                : scalarLocation;
            auto* location = reinterpret_cast<float*>(locationBits);
            float locationArgument;
            std::memcpy(&locationArgument, &locationBits, sizeof(locationArgument));

            bool hit = false;
            std::uint32_t boundaryIndex = 0;
            if (CSF_AT(std::uint32_t, boundaries, 0x8))
            {
                do
                {
                    if (hit)
                        break;

                    if (sweptTest)
                    {
                        alignas(8) std::uint8_t sweepBytes[0x8];
                        std::memcpy(sweepBytes, auxiliary + 4, 0x8);
                        void* boundary =
                            CSF_AT(void**, boundaries, 0x0)[boundaryIndex];
                        hit = GfxParticleRender_TestSweptBoundary(
                            boundary, nullptr, locationArgument,
                            reinterpret_cast<float*>(sweepBytes)) != 0;
                    }
                    else
                    {
                        void* boundary =
                            CSF_AT(void**, boundaries, 0x0)[boundaryIndex];
                        if (RwBBoxContainsPoint(
                                reinterpret_cast<float*>(
                                    static_cast<std::uint8_t*>(boundary) + 0x10),
                                location))
                        {
                            const std::uint32_t faceCount = CSF_AT(
                                std::uint32_t, CSF_AT(void*, boundary, 0x4), 0x18);
                            alignas(8) std::uint8_t pointBytes[0x28];
                            auto* point = reinterpret_cast<float*>(pointBytes);
                            point[0] = location[0];
                            point[1] = location[2];

                            std::uint32_t faceIndex = 0;
                            while (!hit && faceIndex < faceCount)
                            {
                                hit = GfxParticleRender_TestBoundaryFace(
                                    boundary, nullptr,
                                    static_cast<int>(faceIndex++), point) != 0;
                            }
                        }
                    }

                    ++boundaryIndex;
                }
                while (boundaryIndex < CSF_AT(std::uint32_t, boundaries, 0x8));

                if (hit)
                {
                    CSF_AT(std::uint8_t,
                        reinterpret_cast<void*>(colorStream[0]), 0x3) = 0;
                    *reinterpret_cast<float*>(velocityBytes + lifetimeRelativeOffset) =
                        *reinterpret_cast<float*>(velocityBytes + ageRelativeOffset);
                }
            }
        }

        matrixStream[0] += matrixStream[1];
        colorStream[0] += colorStream[1];
        velocityBytes += particleStride;
    }

    return batchAddress;
}

HOOK(0x007047C0, GfxParticleRender_UpdateParticleBatch, "auto");
