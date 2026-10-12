#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GfxTextureLoader_UpdateScaledParticleParameters, 0x006DE030, (void* self, void*));
CSF_FUNC(int*, __fastcall, GfxParticleEmitters_InitializeSettings, 0x006DE210, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float GfxPrtStdParticles_GetExtentProduct(float x, float y, float z)
{
    const float* largest = &z;
    if (!(y < z))
        largest = &y;
    if (!(x < *largest))
        largest = &x;
    const float largestValue = *largest;

    const float* largestYZ = &z;
    if (!(y < z))
        largestYZ = &y;
    const float* largestXY = &y;
    if (!(x < y))
        largestXY = &x;
    if (!(*largestXY < *largestYZ))
        largestXY = largestYZ;

    return largestValue * *largestXY;
}

static float* __fastcall GfxPrtStdParticles_UpdateEmitterShape(
    void* self, void* /*edx*/, float* shape, char scaleParticleCounts)
{
    void* originalSettings = CSF_AT(void*, self, 0x10);
    if (!CSF_AT(std::uint8_t, self, 0x14))
    {
        void* allocation = Mem_Alloc(340);
        int* settings = allocation
            ? GfxParticleEmitters_InitializeSettings(allocation, nullptr)
            : nullptr;

        CSF_AT(void*, self, 0x10) = settings;
        std::memcpy(settings, originalSettings, 0x154);
        CSF_AT(std::uint8_t, self, 0x14) = 1;
    }

    void* settings = CSF_AT(void*, self, 0x10);
    std::memcpy(static_cast<std::uint8_t*>(settings) + 0xD4, shape, 0x2C);

    if (scaleParticleCounts)
    {
        float countScale = g_TransitionOne;
        const int shapeType = CSF_AT(int, originalSettings, 0xD4);
        if (shapeType == 1)
        {
            countScale = CSF_AT(float, settings, 0xE4)
                       / CSF_AT(float, originalSettings, 0xE4);
        }
        else if (shapeType == 0)
        {
            const float originalProduct = GfxPrtStdParticles_GetExtentProduct(
                CSF_AT(float, originalSettings, 0xF4),
                CSF_AT(float, originalSettings, 0xF8),
                CSF_AT(float, originalSettings, 0xFC));
            const float currentProduct = GfxPrtStdParticles_GetExtentProduct(
                CSF_AT(float, settings, 0xF4),
                CSF_AT(float, settings, 0xF8),
                CSF_AT(float, settings, 0xFC));
            countScale = currentProduct / originalProduct;
        }

        CSF_AT(std::uint32_t, settings, 0x0C) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                static_cast<float>(CSF_AT(std::int32_t, settings, 0x0C)) * countScale));
        CSF_AT(std::uint32_t, settings, 0x88) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                static_cast<float>(CSF_AT(std::int32_t, settings, 0x88)) * countScale));

        if (countScale > g_TransitionOne)
        {
            GfxTextureLoader_UpdateScaledParticleParameters(settings, nullptr);
            CSF_AT(std::uint8_t, self, 0x15) = 1;
        }
    }

    float boundsScale = 1.0f;
    const int shapeType = CSF_AT(int, originalSettings, 0xD4);
    if (shapeType == 1)
    {
        boundsScale = CSF_AT(float, settings, 0xE4)
                    / CSF_AT(float, originalSettings, 0xE4);
    }
    else if (shapeType == 0)
    {
        const float x = CSF_AT(float, originalSettings, 0xF4);
        const float y = CSF_AT(float, originalSettings, 0xF8);
        const float z = CSF_AT(float, originalSettings, 0xFC);
        const float length = std::sqrt(
            shape[8] * shape[8] + shape[9] * shape[9] + shape[10] * shape[10]);
        boundsScale = length / std::sqrt(x * x + y * y + z * z);
    }

    alignas(8) std::uint8_t boundsBytes[0x18] = {};
    alignas(8) std::uint8_t centerABytes[0x0C];
    alignas(8) std::uint8_t centerBBytes[0x2C];
    float* centerA = reinterpret_cast<float*>(centerABytes);
    float* centerB = reinterpret_cast<float*>(centerBBytes);

    float* center = SceneMissionSetup_GetBoundsCenter(boundsBytes, nullptr, centerB);
    float scaledX = (CSF_AT(float, settings, 0x13C) - center[0]) * boundsScale;
    float scaledY = (CSF_AT(float, settings, 0x140) - center[1]) * boundsScale;
    float scaledZ = (CSF_AT(float, settings, 0x144) - center[2]) * boundsScale;

    center = SceneMissionSetup_GetBoundsCenter(boundsBytes, nullptr, centerA);
    float resultX = scaledX + center[0];
    float resultY = scaledY + center[1];
    float resultZ = scaledZ + center[2];
    CSF_AT(float, settings, 0x13C) = resultX;
    CSF_AT(float, settings, 0x140) = resultY;
    CSF_AT(float, settings, 0x144) = resultZ;

    center = SceneMissionSetup_GetBoundsCenter(boundsBytes, nullptr, centerA);
    scaledX = (CSF_AT(float, settings, 0x148) - center[0]) * boundsScale;
    scaledY = (CSF_AT(float, settings, 0x14C) - center[1]) * boundsScale;
    scaledZ = (CSF_AT(float, settings, 0x150) - center[2]) * boundsScale;

    center = SceneMissionSetup_GetBoundsCenter(boundsBytes, nullptr, centerB);
    resultX = scaledX + center[0];
    resultY = scaledY + center[1];
    resultZ = scaledZ + center[2];
    CSF_AT(float, settings, 0x148) = resultX;
    CSF_AT(float, settings, 0x14C) = resultY;
    CSF_AT(float, settings, 0x150) = resultZ;

    const float translationX = shape[1] - CSF_AT(float, originalSettings, 0xD8);
    const float translationY = shape[2] - CSF_AT(float, originalSettings, 0xDC);
    const float translationZ = shape[3] - CSF_AT(float, originalSettings, 0xE0);
    CSF_AT(float, settings, 0x13C) += translationX;
    CSF_AT(float, settings, 0x140) += translationY;
    CSF_AT(float, settings, 0x144) += translationZ;
    CSF_AT(float, settings, 0x148) += translationX;
    CSF_AT(float, settings, 0x14C) += translationY;
    CSF_AT(float, settings, 0x150) += translationZ;
    return shape;
}

HOOK(0x006F4020, GfxPrtStdParticles_UpdateEmitterShape, "auto");
