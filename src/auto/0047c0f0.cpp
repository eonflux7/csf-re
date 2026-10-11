#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AiAttackTasks_BuildMovementPath, 0x0047BAD0,
         (float* a1, float arg4, void* a3));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0,
         (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190,
         (C_0086c12c* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360,
         (C_0086c12c* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall C_0082f9f8_StartMovementAlongPath(
    C_0082f9f8* self, void* /*edx*/, float* destination,
    float pathParameter, int durationBits)
{
    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, destination, 0);
    CSF_AT(uint32_t, self, 0x68) = CSF_AT(uint32_t, destination, 4);
    CSF_AT(uint32_t, self, 0x6C) = CSF_AT(uint32_t, destination, 8);

    CSF_AT(float, self, 0x84) = pathParameter;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x2B0) =
        durationBits;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x2AC) = 0;

    alignas(8) uint8_t pathBytes[0x18];
    C_0086c12c& path = *reinterpret_cast<C_0086c12c*>(pathBytes);
    C_0086c12c_Construct(&path, nullptr);
    AiAttackTasks_BuildMovementPath(destination, pathParameter, &path);

    void* movementPath = reinterpret_cast<uint8_t*>(self) + 0x88;
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, movementPath, 0)[0x1C / 4])(
            movementPath, nullptr);

    const uint32_t pointCount = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(GetCachedObjectProvider(&path, nullptr)));

    float totalSquaredLength = 0.0f;
    for (uint32_t index = 1; index < pointCount; ++index)
    {
        float* previous = reinterpret_cast<float*>(
            GuiControl_GetPathPointRecord(&path, nullptr, index - 1));
        float* current = reinterpret_cast<float*>(
            GuiControl_GetPathPointRecord(&path, nullptr, index));

        const float dx = current[0] - previous[0];
        const float dy = current[1] - previous[1];
        const float dz = current[2] - previous[2];
        totalSquaredLength = dz * dz + dy * dy + dx * dx +
                             totalSquaredLength;
    }

    void** movementVtable = CSF_AT(void**, movementPath, 0);
    float accumulatedSquaredLength = 0.0f;
    int firstPoint = GuiControl_GetPathPointRecord(&path, nullptr, 0);
    ((void(__fastcall*)(void*, void*, int, float))
        movementVtable[0x18 / 4])(
            movementPath, nullptr, firstPoint, 0.0f);

    if (pointCount > 1)
    {
        const float inverseSquaredLength =
            g_TransitionOne / totalSquaredLength;

        for (uint32_t index = 1; index < pointCount; ++index)
        {
            float* previous = reinterpret_cast<float*>(
                GuiControl_GetPathPointRecord(&path, nullptr, index - 1));
            float* current = reinterpret_cast<float*>(
                GuiControl_GetPathPointRecord(&path, nullptr, index));

            const float dx = current[0] - previous[0];
            const float dy = current[1] - previous[1];
            const float dz = current[2] - previous[2];
            movementVtable = CSF_AT(void**, movementPath, 0);

            accumulatedSquaredLength = dz * dz + dy * dy + dx * dx +
                                       accumulatedSquaredLength;
            const float time =
                accumulatedSquaredLength *
                CSF_AT(float, reinterpret_cast<void*>(self->field_0008),
                       0x2B0) *
                inverseSquaredLength;

            int point = GuiControl_GetPathPointRecord(&path, nullptr, index);
            ((void(__fastcall*)(void*, void*, int, float))
                movementVtable[0x18 / 4])(
                    movementPath, nullptr, point, time);
        }
    }

    void* character = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0008), 0x14);
    TaskCharAction_48CFE0(
        CSF_AT(void*, character, 0x204), nullptr,
        reinterpret_cast<int>(self), 7, 1);
    CharTask_StartMovement(
        self, nullptr, reinterpret_cast<int>(&self->field_0014));

    return C_0086c12c_ReleaseOwnedBuffer(&path, nullptr);
}

HOOK(0x0047C0F0, C_0082f9f8_StartMovementAlongPath, "auto");
