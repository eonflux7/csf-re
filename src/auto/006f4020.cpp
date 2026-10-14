#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GfxTextureLoader_UpdateScaledParticleParameters, 0x006DE030, (void* self, void*));
CSF_FUNC(int*, __fastcall, S_006dcf90_InitializeSettings, 0x006DE210, (S_006dcf90* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static double GfxPrtStdParticles_GetShapeScaleArea(float x, float y, float z)
{
    const float largerYZ = !(y < z) ? y : z;
    const float largest = !(x < largerYZ) ? x : largerYZ;
    const float largerXY = !(x < y) ? x : y;
    const float other = !(largerXY < largerYZ) ? largerYZ : largerXY;
    return static_cast<double>(largest) * other;
}

static float* __fastcall GfxPrtStdParticles_UpdateEmitterShape(
    void* self, void* /*edx*/, float* shape, char scaleParticleCounts)
{
    void* originalSettings = CSF_AT(void*, self, 0x10);

    if (!CSF_AT(std::uint8_t, self, 0x14))
    {
        const int allocation = CoreMemory_AllocUntagged(340);
        int* settings = allocation
            ? S_006dcf90_InitializeSettings(
                  reinterpret_cast<S_006dcf90*>(allocation), nullptr)
            : nullptr;

        CSF_AT(void*, self, 0x10) = settings;
        std::memcpy(settings, originalSettings, 0x154);
        CSF_AT(std::uint8_t, self, 0x14) = 1;
    }

    void* settings = CSF_AT(void*, self, 0x10);
    std::memcpy(static_cast<std::uint8_t*>(settings) + 0xD4, shape, 0x2C);

    if (scaleParticleCounts)
    {
        double countScale = g_TransitionOne;
        const int originalShapeType = CSF_AT(int, originalSettings, 0xD4);

        if (originalShapeType == 1)
        {
            countScale = static_cast<double>(CSF_AT(float, settings, 0xE4))
                       / CSF_AT(float, originalSettings, 0xE4);
        }
        else if (originalShapeType == 0)
        {
            const float oldX = CSF_AT(float, originalSettings, 0xF4);
            const float oldY = CSF_AT(float, originalSettings, 0xF8);
            const float oldZ = CSF_AT(float, originalSettings, 0xFC);
            const double oldArea =
                GfxPrtStdParticles_GetShapeScaleArea(oldX, oldY, oldZ);

            const float newX = CSF_AT(float, settings, 0xF4);
            const float newY = CSF_AT(float, settings, 0xF8);
            const float newZ = CSF_AT(float, settings, 0xFC);
            countScale =
                GfxPrtStdParticles_GetShapeScaleArea(newX, newY, newZ) / oldArea;
        }

        const std::int64_t scaledCount = static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, settings, 0x0C)) * countScale);
        const std::int32_t secondaryCount =
            CSF_AT(std::int32_t, settings, 0x88);

        CSF_AT(std::int32_t, settings, 0x0C) =
            static_cast<std::int32_t>(scaledCount);
        CSF_AT(std::int32_t, settings, 0x88) =
            static_cast<std::int32_t>(static_cast<std::int64_t>(
                static_cast<double>(secondaryCount) * countScale));

        if (countScale > g_TransitionOne)
        {
            GfxTextureLoader_UpdateScaledParticleParameters(settings, nullptr);
            CSF_AT(std::uint8_t, self, 0x15) = 1;
        }
    }

    float boundsScale = 1.0f;
    const int originalShapeType = CSF_AT(int, originalSettings, 0xD4);
    if (originalShapeType == 1)
    {
        boundsScale = static_cast<double>(CSF_AT(float, settings, 0xE4))
                    / CSF_AT(float, originalSettings, 0xE4);
    }
    else if (originalShapeType == 0)
    {
        const float oldZ = CSF_AT(float, originalSettings, 0xFC);
        const float oldY = CSF_AT(float, originalSettings, 0xF8);
        const float oldX = CSF_AT(float, originalSettings, 0xF4);
        const float newLength = std::sqrt(
            static_cast<double>(shape[8]) * shape[8] +
            static_cast<double>(shape[9]) * shape[9] +
            static_cast<double>(shape[10]) * shape[10]);
        boundsScale = newLength
                    / std::sqrt(static_cast<double>(oldX) * oldX +
                                static_cast<double>(oldY) * oldY +
                                static_cast<double>(oldZ) * oldZ);
    }

    alignas(8) std::uint8_t boundsBytes[0x18];
    alignas(8) std::uint8_t centerABytes[0xC];
    alignas(8) std::uint8_t centerBBytes[0x2C];

    float* bounds = reinterpret_cast<float*>(boundsBytes);
    // EBX is the pre-clone settings; the bbox holds max followed by min.
    bounds[0] = CSF_AT(float, originalSettings, 0x148);
    bounds[1] = CSF_AT(float, originalSettings, 0x14C);
    bounds[2] = CSF_AT(float, originalSettings, 0x150);
    bounds[3] = CSF_AT(float, originalSettings, 0x13C);
    bounds[4] = CSF_AT(float, originalSettings, 0x140);
    bounds[5] = CSF_AT(float, originalSettings, 0x144);

    float* center = SceneMissionSetup_GetBoundsCenter(
        boundsBytes, nullptr, reinterpret_cast<float*>(centerBBytes));

    const float firstDeltaZ = CSF_AT(float, settings, 0x144) - center[2];
    const float firstX = static_cast<float>(
        (static_cast<double>(CSF_AT(float, settings, 0x13C)) - center[0]) * boundsScale);
    const float firstY = static_cast<float>(
        (static_cast<double>(CSF_AT(float, settings, 0x140)) - center[1]) * boundsScale);
    const float firstZ = firstDeltaZ * boundsScale;

    center = SceneMissionSetup_GetBoundsCenter(
        boundsBytes, nullptr, reinterpret_cast<float*>(centerABytes));

    const float scaledFirstX = firstX + center[0];
    const float scaledFirstY = firstY + center[1];
    const float scaledFirstZ = firstZ + center[2];
    CSF_AT(float, settings, 0x13C) = scaledFirstX;
    CSF_AT(float, settings, 0x140) = scaledFirstY;
    CSF_AT(float, settings, 0x144) = scaledFirstZ;

    center = SceneMissionSetup_GetBoundsCenter(
        boundsBytes, nullptr, reinterpret_cast<float*>(centerABytes));

    const float secondDeltaZ = CSF_AT(float, settings, 0x150) - center[2];
    const float secondX = static_cast<float>(
        (static_cast<double>(CSF_AT(float, settings, 0x148)) - center[0]) * boundsScale);
    const float secondY = static_cast<float>(
        (static_cast<double>(CSF_AT(float, settings, 0x14C)) - center[1]) * boundsScale);
    const float secondZ = secondDeltaZ * boundsScale;

    center = SceneMissionSetup_GetBoundsCenter(
        boundsBytes, nullptr, reinterpret_cast<float*>(centerBBytes));

    const float scaledSecondX = secondX + center[0];
    const float scaledSecondY = secondY + center[1];
    const float scaledSecondZ = secondZ + center[2];
    CSF_AT(float, settings, 0x148) = scaledSecondX;
    CSF_AT(float, settings, 0x14C) = scaledSecondY;
    CSF_AT(float, settings, 0x150) = scaledSecondZ;

    // Capture operands before any bounds stores (shape can alias settings).
    // Keep X/Y subtraction and addition in one expression: the original does
    // not spill those differences before FADD/FSTP at 006F43DF..006F43FF.
    const float shapeX = shape[1], shapeY = shape[2];
    const float oldShapeX = CSF_AT(float, originalSettings, 0xD8);
    const float oldShapeY = CSF_AT(float, originalSettings, 0xDC);
    const float deltaZ = shape[3] - CSF_AT(float, originalSettings, 0xE0);

    CSF_AT(float, settings, 0x13C) =
        static_cast<float>((static_cast<double>(shapeX) - oldShapeX) + CSF_AT(float, settings, 0x13C));
    CSF_AT(float, settings, 0x140) =
        static_cast<float>((static_cast<double>(shapeY) - oldShapeY) + CSF_AT(float, settings, 0x140));
    CSF_AT(float, settings, 0x144) = deltaZ + CSF_AT(float, settings, 0x144);
    CSF_AT(float, settings, 0x148) =
        static_cast<float>((static_cast<double>(shapeX) - oldShapeX) + CSF_AT(float, settings, 0x148));
    CSF_AT(float, settings, 0x14C) =
        static_cast<float>((static_cast<double>(shapeY) - oldShapeY) + CSF_AT(float, settings, 0x14C));
    CSF_AT(float, settings, 0x150) = deltaZ + CSF_AT(float, settings, 0x150);

    return shape;
}

HOOK(0x006F4020, GfxPrtStdParticles_UpdateEmitterShape, "auto");
