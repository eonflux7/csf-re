#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoint, 0x00623010, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rwFrameSyncDirty, 0x006327D0, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float* __fastcall CCamera_ProjectPointToViewport(
    CCamera* self, void* /*edx*/, float* output, int point)
{
    _rwFrameSyncDirty();

    alignas(8) uint8_t transformBytes[0x18];
    float& transformedX = CSF_AT(float, transformBytes, 0x00);
    float& transformedY = CSF_AT(float, transformBytes, 0x04);
    float& transformedZ = CSF_AT(float, transformBytes, 0x08);
    float& projectedX = CSF_AT(float, transformBytes, 0x0C);
    float& viewportHeight = CSF_AT(float, transformBytes, 0x10);
    float& viewportWidth = CSF_AT(float, transformBytes, 0x14);

    viewportWidth = static_cast<float>(
        static_cast<int32_t>(self->field_003c - self->field_0034));
    viewportHeight = static_cast<float>(
        static_cast<int32_t>(self->field_0040 - self->field_0038));

    transformedX = 0.0f;
    transformedY = 0.0f;
    transformedZ = 0.0f;

    RwV3dTransformPoint(
        reinterpret_cast<int>(transformBytes),
        point,
        static_cast<int>(self->field_000c + 32));

    if (self->field_004c == 1)
    {
        projectedX = static_cast<float>(static_cast<int32_t>(self->field_0034))
                   + transformedX * viewportWidth;
        viewportHeight = static_cast<float>(static_cast<int32_t>(self->field_0038))
                       + transformedY * viewportHeight;
    }
    else
    {
        const float inverseDepth = g_TransitionOne / transformedZ;
        viewportHeight = inverseDepth * transformedY * viewportHeight;
        projectedX = static_cast<float>(static_cast<int32_t>(self->field_0034))
                   + transformedX * inverseDepth * viewportWidth;
        viewportHeight = static_cast<float>(static_cast<int32_t>(self->field_0038))
                       + viewportHeight;
    }

    output[0] = projectedX;
    output[1] = viewportHeight;
    return output;
}

HOOK(0x006D2E90, CCamera_ProjectPointToViewport, "auto");
