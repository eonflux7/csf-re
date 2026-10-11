#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_IntersectRaySphere_6ACDF0, 0x006ACDF0, (float* self, void* , float* sphere, float* nearDistance, float* farDistance));
CSF_FUNC(int, __fastcall, C_0086379c_GetIntersectionSphere, 0x0070BE30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall C_0086379c_IntersectScaledSphere(
    C_0086379c* self, void* /*edx*/, float* inputRay,
    float* output, float expansion)
{
    alignas(8) uint8_t rayBytes[0x18];
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t directionOutputBytes[0xC];
    alignas(8) uint8_t workspaceBytes[0x30];

    float* ray = reinterpret_cast<float*>(rayBytes);
    float* sphere = reinterpret_cast<float*>(sphereBytes);
    uint32_t* secondDirectionOutput =
        reinterpret_cast<uint32_t*>(workspaceBytes);
    float* direction = reinterpret_cast<float*>(workspaceBytes + 0xC);
    float* origin = reinterpret_cast<float*>(workspaceBytes + 0x18);
    float* scale = reinterpret_cast<float*>(workspaceBytes + 0x24);
    float* worldOrigin = inputRay + 3;

    C_0086379c_GetIntersectionSphere(
        self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(sphere)));

    const float expansionFactor = expansion + g_TransitionOne;
    scale[0] = expansionFactor * self->field_0014;
    origin[0] = inputRay[3];
    origin[1] = inputRay[4];
    scale[1] = expansionFactor * self->field_0018;
    origin[2] = inputRay[5];
    direction[0] = inputRay[0];
    direction[1] = inputRay[1];
    direction[2] = inputRay[2];
    scale[2] = expansionFactor * self->field_001c;

    const float inverseX = g_TransitionOne / scale[0];
    origin[0] *= inverseX;
    ray[3] = origin[0];

    const float inverseY = g_TransitionOne / scale[1];
    origin[1] *= inverseY;
    ray[4] = origin[1];

    const float inverseZ = g_TransitionOne / scale[2];
    expansion = inverseZ;
    origin[2] *= inverseZ;
    ray[5] = origin[2];

    direction[0] *= inverseX;
    ray[0] = direction[0];
    direction[1] *= inverseY;
    ray[1] = direction[1];
    direction[2] *= expansion;
    ray[2] = direction[2];

    NormalizeVector3(reinterpret_cast<C_00833b88*>(rayBytes), nullptr);

    float farDistance = 0.0f;
    const int intersection = GfxMatrix_IntersectRaySphere_6ACDF0(
        ray, nullptr, sphere, &expansion, &farDistance);

    uint32_t* convertedDirection;
    if (intersection == 2)
    {
        origin[2] = ray[2] * expansion;
        direction[0] = ray[3] + ray[0] * expansion;
        output[0] = direction[0];
        direction[1] = ray[4] + ray[1] * expansion;
        output[1] = direction[1];
        direction[2] = ray[5] + origin[2];
        output[2] = direction[2];

        direction[0] = output[0] - sphere[0];
        direction[1] = output[1] - sphere[1];
        direction[2] = output[2] - sphere[2];
        convertedDirection = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(direction), nullptr,
            secondDirectionOutput);
    }
    else if (intersection == 1)
    {
        output[0] = ray[3];
        output[1] = ray[4];
        output[2] = ray[5];

        origin[0] = output[0] - sphere[0];
        output[6] = 0.0f;
        origin[1] = output[1] - sphere[1];
        origin[2] = output[2] - sphere[2];
        convertedDirection = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(origin), nullptr,
            reinterpret_cast<uint32_t*>(directionOutputBytes));
    }
    else
    {
        return 0;
    }

    const uint32_t normalX = convertedDirection[0];
    const float worldX = scale[0] * output[0];
    CSF_AT(uint32_t, output, 0xC) = normalX;
    CSF_AT(uint32_t, output, 0x10) = convertedDirection[1];
    const uint32_t normalZ = convertedDirection[2];
    output[0] = worldX;
    CSF_AT(uint32_t, output, 0x14) = normalZ;

    const float worldY = scale[1] * output[1];
    output[1] = worldY;
    const float worldZ = scale[2] * output[2];
    output[2] = worldZ;
    output[3] *= scale[0];
    output[4] *= scale[1];
    output[5] *= scale[2];

    if (intersection == 2)
    {
        direction[0] = worldOrigin[0] - worldX;
        const float deltaY = worldOrigin[1] - worldY;
        const float deltaZ = worldOrigin[2] - worldZ;
        output[6] = std::sqrt(
            deltaZ * deltaZ + deltaY * deltaY +
            direction[0] * direction[0]);
    }

    return 1;
}

HOOK(0x0070C010, C_0086379c_IntersectScaledSphere, "auto");
