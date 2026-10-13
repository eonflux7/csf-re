#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(bool, __fastcall, GfxMatrix_ForwardValueWithAddress, 0x006ACA20, (void* self, void* , int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __stdcall ButtonSelection_MatchesPosition(
    C_008342e8* selection, float* position, float height)
{
    void* attached = reinterpret_cast<void*>(selection->field_004c);
    if (!attached)
        return false;

    void* object = C_0082dd78_DispatchReturnedObject(attached, nullptr);
    if (!object)
        return false;

    void* transformSource =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[0x34 / 4])(
            object, nullptr);
    if (!transformSource)
        return false;

    alignas(8) uint8_t transformBytes[0x3C];
    S_006abb80& transform = *reinterpret_cast<S_006abb80*>(transformBytes);

    ((void (__fastcall*)(void*, void*, S_006abb80*))
        CSF_AT(void**, transformSource, 0)[0x1C / 4])(
            transformSource, nullptr, &transform);

    const float radius = position[3];
    const float radiusSquared = radius * radius;

    if (height == g_fZero)
    {
        const float dx = position[0] - transform.field_0000;
        const float dy = position[1] - transform.field_0004;
        const float dz = position[2] - transform.field_0008;

        float* dimensions =
            ((float* (__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);
        const float sizeZ = dimensions[2];
        const float sizeY = dimensions[1];
        const float sizeX = dimensions[0];

        const float distanceSquared = dz * dz + dy * dy + dx * dx;
        const float limitSquared =
            sizeX * sizeX + sizeY * sizeY + sizeZ * sizeZ + radiusSquared;

        return limitSquared > distanceSquared &&
            GfxMatrix_ForwardValueWithAddress(
                &transform, nullptr, reinterpret_cast<int>(position));
    }

    alignas(8) uint8_t maxXBytes[0x4];
    alignas(8) uint8_t minXBytes[0x4];
    alignas(8) uint8_t maxYBytes[0x4];
    alignas(8) uint8_t minYBytes[0x20];
    alignas(8) uint8_t maxZBytes[0x4];
    alignas(8) uint8_t minZBytes[0x4];

    float& maxY = *reinterpret_cast<float*>(maxYBytes);
    float& minY = *reinterpret_cast<float*>(minYBytes);

    S_006abb80_ComputeAxisAlignedBounds(
        &transform, nullptr,
        reinterpret_cast<float*>(maxXBytes),
        reinterpret_cast<float*>(minXBytes),
        &maxY, &minY,
        reinterpret_cast<float*>(maxZBytes),
        reinterpret_cast<float*>(minZBytes));

    const float baseX = position[0];
    const float baseY = position[1];
    const float baseZ = position[2];
    const float elevatedY = position[1] + height;
    const float elevatedX = position[0];
    const float elevatedZ = position[2];

    float* objectPosition = GetObjectPositionVector(selection, nullptr);
    const float centerX = objectPosition[0];
    const float centerY = objectPosition[1];
    const float centerZ = objectPosition[2];
    const float alternateX = objectPosition[0];
    const float alternateY = objectPosition[1];
    const float alternateZ = objectPosition[2];
    (void)alternateY;

    alignas(8) uint8_t displacementBytes[0x18];
    float* displacement = reinterpret_cast<float*>(displacementBytes);

    if (minY > elevatedY)
    {
        displacement[0] = centerX - elevatedX;
        displacement[1] = centerY - elevatedY;
        displacement[2] = centerZ - elevatedZ;
    }
    else if (maxY < baseY)
    {
        displacement[0] = alternateX - baseX;
        displacement[1] = maxY - baseY;
        displacement[2] = alternateZ - baseZ;
    }
    else
    {
        displacement[0] = centerX - baseX;
        displacement[1] = 0.0f;
        displacement[2] = centerZ - baseZ;
    }

    const float distanceSquared =
        static_cast<float>(CalculateSquaredVectorLength(displacement, nullptr));

    float* dimensions =
        ((float* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);
    const float sizeZ = dimensions[2];
    const float sizeY = dimensions[1];
    const float sizeX = dimensions[0];

    const float limitSquared =
        (sizeX * sizeX + sizeY * sizeY + sizeZ * sizeZ) *
            g_HalfDurationScale + radiusSquared;

    return distanceSquared < limitSquared;
}

HOOK(0x0044F3E0, ButtonSelection_MatchesPosition, "auto");
