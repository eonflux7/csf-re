#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(float*, __stdcall, GfxClumpLoader_GetPartReferencePosition, 0x006FE5F0, (float* a1, int a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, GfxClumpLoader_GrowPartResults, 0x006FE780, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, C_00863044_EvaluatePartBounds, 0x006FEAE0, (void* self, void* edx, float* a2, int* a3));
CSF_FUNC(int, __cdecl, GfxClumpLoader_GetObjectBoundsData, 0x007F7BF0, (uint32_t* a1, uint32_t* a2, int a3, int a4));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Initialization references supplied by the packet; the bound callees
// perform the initialization, so no additional stores are needed here.
static const auto g_TransformInitializationVtable = CSF_VTBL(0x00402350);
static const auto g_BoundsInitializationVtable = CSF_VTBL(0x006ABB80);

static int __fastcall C_00863044_CollectWeightedClumpParts(
    C_00863044* self, void* /*edx*/, uint32_t* clump, uint32_t* results)
{
    auto* partData = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(clump[0]), 0x4));
    const int partCount = CSF_AT(uint16_t, partData, 0xC);

    alignas(8) uint8_t referencePositionBytes[0xC];
    alignas(8) uint8_t transformBytes[0x60];
    alignas(8) uint8_t objectBoundsBytes[0x50];
    alignas(8) uint8_t matrixStateBytes[0x14];

    auto& transform = *reinterpret_cast<S_00402350*>(transformBytes);

    int entryOffset = 0;
    for (int partIndex = 0; partIndex < partCount; ++partIndex)
    {
        const uint32_t objectAddress = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(CSF_AT(uint32_t, partData, 0x10)),
            entryOffset);

        S00402350_InitTransform(&transform, nullptr);
        GfxClumpLoader_GetObjectBoundsData(
            reinterpret_cast<uint32_t*>(objectAddress), nullptr, 0,
            reinterpret_cast<int>(objectBoundsBytes));

        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
            CSF_AT(float, objectBoundsBytes, 0x0),
            CSF_AT(float, objectBoundsBytes, 0x10),
            CSF_AT(float, objectBoundsBytes, 0x4),
            CSF_AT(float, objectBoundsBytes, 0x14),
            CSF_AT(float, objectBoundsBytes, 0x8),
            CSF_AT(float, objectBoundsBytes, 0x18));

        CSF_AT(uint32_t, matrixStateBytes, 0x0) = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(CSF_AT(uint32_t, partData, 0x10)),
            entryOffset + 8);
        CSF_AT(uint8_t, matrixStateBytes, 0x4) = 0;

        GfxMatrix_TransformBasisAndPosition(
            reinterpret_cast<uint32_t*>(transformBytes), nullptr,
            reinterpret_cast<uint32_t*>(matrixStateBytes));

        self->field_005c = 0;
        const double evaluatedWeight = C_00863044_EvaluatePartBounds(
            self, nullptr, reinterpret_cast<float*>(&transform.field_0000),
            reinterpret_cast<int*>(&self->field_0054));
        const float weight = static_cast<float>(evaluatedWeight);

        if (evaluatedWeight > g_fZero && self->field_005c)
        {
            void* clumpObject = reinterpret_cast<void*>(clump[0]);
            const double partWeight =
                reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, clumpObject, 0)[0x5C / 4])(
                    clumpObject, nullptr, partIndex);
            const float scaledWeight = static_cast<float>(partWeight * weight);

            uint32_t xBits = transform.field_0000;
            uint32_t yBits = transform.field_0004;
            uint32_t zBits = transform.field_0008;

            // The original takes the blending path only for an ordered
            // comparison below one.
            if (weight < g_TransitionOne)
            {
                float x, y, z;
                std::memcpy(&x, &xBits, sizeof(x));
                std::memcpy(&y, &yBits, sizeof(y));
                std::memcpy(&z, &zBits, sizeof(z));

                float* referencePosition =
                    GfxClumpLoader_GetPartReferencePosition(
                        reinterpret_cast<float*>(referencePositionBytes),
                        reinterpret_cast<int>(clump), &self->field_0054);

                const float referenceXHalf =
                    referencePosition[0] * g_HalfDurationScale;
                const float referenceYHalf =
                    referencePosition[1] * g_HalfDurationScale;
                const float referenceZHalf =
                    referencePosition[2] * g_HalfDurationScale;

                x = x * g_HalfDurationScale + referenceXHalf;
                y = y * g_HalfDurationScale + referenceYHalf;
                z = z * g_HalfDurationScale + referenceZHalf;

                std::memcpy(&xBits, &x, sizeof(xBits));
                std::memcpy(&yBits, &y, sizeof(yBits));
                std::memcpy(&zBits, &z, sizeof(zBits));
            }

            if (results[2] >= results[1])
                GfxClumpLoader_GrowPartResults(
                    results, nullptr, static_cast<int>(results[3]));

            const uint32_t resultIndex = results[2];
            auto* result = reinterpret_cast<uint8_t*>(results[0])
                         + 24 * resultIndex;
            results[2] = resultIndex + 1;

            CSF_AT(int, result, 0x0) = partIndex;
            CSF_AT(float, result, 0x4) = scaledWeight;
            CSF_AT(float, result, 0x8) = weight;
            CSF_AT(uint32_t, result, 0xC) = xBits;
            CSF_AT(uint32_t, result, 0x10) = yBits;
            CSF_AT(uint32_t, result, 0x14) = zBits;
        }

        if (CSF_AT(uint8_t, matrixStateBytes, 0x4))
            RwMatrixDestroy(reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, matrixStateBytes, 0x0)));

        entryOffset += 12;
    }

    return results[2] != 0;
}

HOOK(0x006FF080, C_00863044_CollectWeightedClumpParts, "auto");
