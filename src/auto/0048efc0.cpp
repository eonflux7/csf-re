#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, IsField400NonZero, 0x00495E70, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_AppendEventRecord, 0x0049AB60, (C_00833908* self, void* , int a2));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));

CSF_GLOBAL(float, g_CoverAttackFarDistanceSquared, 0x0082D4C0);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x008339A8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833908_UpdateProximityEvents(
    C_00833908* self, void* /*edx*/, float elapsed)
{
    auto* sceneList = static_cast<std::uint8_t*>(g_pCurrentScene) + 0x60;
    const std::uint32_t initialCount = CSF_AT(std::uint32_t, sceneList, 0x08);
    if (initialCount == 0)
        return 0;

    bool eligibilityChecked = false;
    std::uint8_t eligible = 1;
    auto* timer = &self->field_0178;
    std::uint32_t index = 0;

    do
    {
        const float remaining = CSF_AT(float, timer, 0) - elapsed;
        CSF_AT(float, timer, 0) = remaining;

        if (remaining > g_fZero)
        {
            auto* other = CSF_AT(
                C_00833908*,
                CSF_AT(void*, sceneList, 0),
                index * 4);
            if (static_cast<std::uint8_t>(IsField400NonZero(other, nullptr)) == 0)
                goto nextObject;
        }

        *timer = 0;
        if (!eligibilityChecked)
        {
            eligibilityChecked = true;
            eligible = static_cast<std::uint8_t>(self->vSlot9C());
        }

        {
            float sphere[0x2C / sizeof(float)] = {};
            float matrix[0x4C / sizeof(float)] = {};

            if (!eligible)
            {
                CSF_AT(float, timer, 0) = 5.0f;
                goto nextObject;
            }

            auto* other = CSF_AT(
                C_00833908*,
                CSF_AT(void*, sceneList, 0),
                index * 4);
            if (other == self)
                goto nextObject;

            if (!self->field_016c || self->field_0050 == 34)
            {
                void* dispatched = reinterpret_cast<void*>(
                    C_0082dd78_DispatchReturnedObject(
                        reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));

                void* matrixSource =
                    ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, dispatched, 0)[0x34 / 4])(
                            dispatched, nullptr);

                float* transformedMatrix =
                    ((float*(__fastcall*)(void*, void*, float*))
                        CSF_AT(void**, matrixSource, 0)[0x1C / 4])(
                            matrixSource, nullptr, matrix);

                GfxMatrix_GetBoundingSphere(transformedMatrix, nullptr, sphere);
            }
            else
            {
                void* boundsObject = reinterpret_cast<void*>(self->field_016c);
                float* bounds =
                    ((float*(__fastcall*)(void*, void*))
                        CSF_AT(void**, boundsObject, 0)[0x28 / 4])(
                            boundsObject, nullptr);

                sphere[0] = CSF_AT(float, bounds, 0);
                sphere[1] = CSF_AT(float, bounds, 0x04);
                sphere[2] = CSF_AT(float, bounds, 0x08);
                sphere[3] = CSF_AT(float, bounds, 0x0C);
            }

            void* otherBoundsObject =
                reinterpret_cast<void*>(other->vGetField016c());
            float* otherBounds =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, otherBoundsObject, 0)[0x28 / 4])(
                        otherBoundsObject, nullptr);

            const float combinedRadius =
                CSF_AT(float, otherBounds, 0x0C) + sphere[3];

            void* ownObject = reinterpret_cast<void*>(self->vGetField016c());
            void* ownTransform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, ownObject, 0)[0x18 / 4])(
                        ownObject, nullptr);

            void* otherObject = reinterpret_cast<void*>(other->vGetField016c());
            void* otherTransform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, otherObject, 0)[0x18 / 4])(
                        otherObject, nullptr);

            float* otherPosition =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, otherTransform, 0)[0x10 / 4])(
                        otherTransform, nullptr);
            float* ownPosition =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, ownTransform, 0)[0x10 / 4])(
                        ownTransform, nullptr);

            const float dx = otherPosition[0] - ownPosition[0];
            const float dy = otherPosition[1] - ownPosition[1];
            const float dz = otherPosition[2] - ownPosition[2];
            const float distanceSquared = dz * dz + dy * dy + dx * dx;

            if (combinedRadius * combinedRadius > distanceSquared)
                C_00833908_AppendEventRecord(
                    other, nullptr, reinterpret_cast<int>(self));

            if (distanceSquared == g_MissingMetricValue)
            {
                CSF_AT(float, timer, 0) = 5.0f;
            }
            else if (!(sphere[3] * sphere[3] > distanceSquared))
            {
                if (distanceSquared > g_CoverAttackFarDistanceSquared)
                    CSF_AT(float, timer, 0) = 1.0f;
                else if (distanceSquared > g_ProximityMidDistanceSquared)
                    CSF_AT(float, timer, 0) = 0.5f;
                else
                    *timer = 0;
            }
            else
            {
                *timer = 0;
            }
        }

    nextObject:
        ++index;
        ++timer;
    }
    while (index < CSF_AT(std::uint32_t, sceneList, 0x08));

    return static_cast<int>(index);
}

HOOK(0x0048EFC0, C_00833908_UpdateProximityEvents, "auto");
