#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalNestedField14, 0x0044F3D0, ());
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190, (C_0086c12c* self, void*));
CSF_FUNC(int, __fastcall, ForwardToInnerWithZeroFlag, 0x0075A340, (void* self, void* , uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (C_0086c12c* self, void*));
CSF_FUNC(char, __fastcall, Navigation_FindPath, 0x0075FC40, (void* self, void* edx, float* arg0, float* arg4, void* a4, uint32_t* a5));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0082f9f8_BuildTimedMovementPath(
    C_0082f9f8* self, void* /*edx*/,
    float* start, float* destination, int duration)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    auto* storedStart = reinterpret_cast<float*>(bytes + 0x64);
    auto* storedDestination = reinterpret_cast<float*>(bytes + 0x70);
    void* movementPath = bytes + 0x88;

    CSF_AT(float, self, 0x64) = start[0];
    CSF_AT(float, self, 0x68) = start[1];
    CSF_AT(float, self, 0x6C) = start[2];
    CSF_AT(float, self, 0x70) = destination[0];
    CSF_AT(float, self, 0x74) = destination[1];
    void* state = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(float, self, 0x78) = destination[2];
    CSF_AT(int, state, 0x2B0) = duration;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x2AC) = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, movementPath, 0)[0x1C / 4])(movementPath, nullptr);

    alignas(8) uint8_t pathBytes[0x14];
    auto& path = *reinterpret_cast<C_0086c12c*>(pathBytes);
    alignas(8) uint8_t resultBytes[0xC] = {};
    auto* pathResult = reinterpret_cast<uint32_t*>(resultBytes);

    C_0086c12c_Construct(&path, nullptr);
    (void)g_pCurrentScene;
    void* navigation = reinterpret_cast<void*>(GetGlobalNestedField14());
    if (Navigation_FindPath(
            navigation, nullptr, storedStart, storedDestination, &path, pathResult))
    {
        ForwardToInnerWithZeroFlag(
            &path, nullptr, reinterpret_cast<uint32_t*>(storedStart));
        const uint32_t pointCount =
            reinterpret_cast<uintptr_t>(GetCachedObjectProvider(&path, nullptr));

        float totalSquaredLength = 0.0f;
        for (uint32_t index = 1; index < pointCount; ++index)
        {
            auto* previous = reinterpret_cast<float*>(
                GuiControl_GetPathPointRecord(&path, nullptr, index - 1));
            auto* current = reinterpret_cast<float*>(
                GuiControl_GetPathPointRecord(&path, nullptr, index));
            float dx = current[0] - previous[0];
            float dy = current[1] - previous[1];
            float dz = current[2] - previous[2];
            totalSquaredLength =
                dz * dz + dy * dy + dx * dx + totalSquaredLength;
        }

        void* appendPoint = CSF_AT(void**, movementPath, 0)[0x18 / 4];
        float accumulatedSquaredLength = 0.0f;
        auto* firstPoint = reinterpret_cast<float*>(
            GuiControl_GetPathPointRecord(&path, nullptr, 0));
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            appendPoint)(movementPath, nullptr, firstPoint, 0.0f);

        if (pointCount > 1)
        {
            const float inverseLength = g_TransitionOne / totalSquaredLength;
            for (uint32_t index = 1; index < pointCount; ++index)
            {
                auto* previous = reinterpret_cast<float*>(
                    GuiControl_GetPathPointRecord(&path, nullptr, index - 1));
                auto* current = reinterpret_cast<float*>(
                    GuiControl_GetPathPointRecord(&path, nullptr, index));
                float dx = current[0] - previous[0];
                float dy = current[1] - previous[1];
                float dz = current[2] - previous[2];
                state = reinterpret_cast<void*>(self->field_0008);
                appendPoint = CSF_AT(void**, movementPath, 0)[0x18 / 4];
                accumulatedSquaredLength =
                    dz * dz + dy * dy + dx * dx + accumulatedSquaredLength;
                float pointTime =
                    accumulatedSquaredLength * CSF_AT(float, state, 0x2B0) *
                    inverseLength;
                auto* point = reinterpret_cast<float*>(
                    GuiControl_GetPathPointRecord(&path, nullptr, index));
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                    appendPoint)(movementPath, nullptr, point, pointTime);
            }
        }
    }
    else
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            CSF_AT(void**, movementPath, 0)[0x18 / 4])(
                movementPath, nullptr, storedStart, 0.0f);

        state = reinterpret_cast<void*>(self->field_0008);
        float endTime = CSF_AT(float, state, 0x2B0);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            CSF_AT(void**, movementPath, 0)[0x18 / 4])(
                movementPath, nullptr, storedDestination, endTime);
    }

    state = reinterpret_cast<void*>(self->field_0008);
    void* character = CSF_AT(void*, state, 0x14);
    void* characterTask = CSF_AT(void*, character, 0x204);
    TaskCharAction_48CFE0(
        characterTask, nullptr, reinterpret_cast<int>(self), 7, 1);
    CharTask_StartMovement(self, nullptr, reinterpret_cast<int>(bytes + 0x14));
    return C_0086c12c_ReleaseOwnedBuffer(&path, nullptr);
}

HOOK(0x0047B730, C_0082f9f8_BuildTimedMovementPath, "auto");
