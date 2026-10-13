#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalNestedField14, 0x0044F3D0, ());
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* , int taskIndex, int orderAddress));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetPathPointRecord, 0x004A0F30, (C_0085e7b8* self, void* , int index));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190, (C_0086c12c* self, void*));
CSF_FUNC(int, __fastcall, ForwardToInnerWithZeroFlag, 0x0075A340, (void* self, void* , uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (C_0086c12c* self, void*));
CSF_FUNC(char, __fastcall, S_0075f7c0_FindPath_75FC40, 0x0075FC40, (S_0075f7c0* self, void* , float* origin, float* destination, C_0085e7b8* path, uint32_t* destinationValue));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083ac40_BuildMovementPath(C_0083ac40* self, void* /*edx*/)
{
    void* trajectory = &self->field_00a4;
    void** trajectoryVtable = CSF_AT(void**, trajectory, 0);
    self->field_0084 = 0.0f;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        trajectoryVtable[0x1C / 4])(trajectory, nullptr);

    alignas(8) uint8_t pathBytes[0x14];
    auto& path = *reinterpret_cast<C_0085e7b8*>(pathBytes);
    alignas(8) uint8_t destinationValueBytes[0x18];
    C_0086c12c_Construct(reinterpret_cast<C_0086c12c*>(&path), nullptr);

    (void)g_pCurrentScene;
    auto* pathFinder = reinterpret_cast<S_0075f7c0*>(GetGlobalNestedField14());
    if (S_0075f7c0_FindPath_75FC40(
            pathFinder, nullptr,
            reinterpret_cast<float*>(&self->field_008c),
            reinterpret_cast<float*>(&self->field_0098),
            &path, reinterpret_cast<uint32_t*>(destinationValueBytes)))
    {
        ForwardToInnerWithZeroFlag(&path, nullptr, &self->field_008c);
        const uint32_t pointCount =
            reinterpret_cast<uint32_t>(GetCachedObjectProvider(&path, nullptr));

        float totalSquaredLength = 0.0f;
        for (uint32_t index = 1; index < pointCount; ++index)
        {
            auto* previous = reinterpret_cast<float*>(
                C_0085e7b8_GetPathPointRecord(
                    &path, nullptr, static_cast<int>(index - 1)));
            auto* current = reinterpret_cast<float*>(
                C_0085e7b8_GetPathPointRecord(
                    &path, nullptr, static_cast<int>(index)));
            const float dx = current[0] - previous[0];
            const float dy = current[1] - previous[1];
            const float dz = current[2] - previous[2];
            totalSquaredLength = dz * dz + dy * dy + dx * dx + totalSquaredLength;
        }

        trajectoryVtable = CSF_AT(void**, trajectory, 0);
        float accumulatedSquaredLength = 0.0f;
        auto* firstPoint = reinterpret_cast<float*>(
            C_0085e7b8_GetPathPointRecord(&path, nullptr, 0));
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            trajectoryVtable[0x18 / 4])(trajectory, nullptr, firstPoint, 0.0f);

        if (pointCount > 1)
        {
            const float inverseSquaredLength = g_TransitionOne / totalSquaredLength;
            for (uint32_t index = 1; index < pointCount; ++index)
            {
                auto* previous = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        &path, nullptr, static_cast<int>(index - 1)));
                auto* current = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        &path, nullptr, static_cast<int>(index)));
                const float dx = current[0] - previous[0];
                const float dy = current[1] - previous[1];
                const float dz = current[2] - previous[2];
                trajectoryVtable = CSF_AT(void**, trajectory, 0);
                accumulatedSquaredLength =
                    dz * dz + dy * dy + dx * dx + accumulatedSquaredLength;
                const float time =
                    inverseSquaredLength * accumulatedSquaredLength * self->field_0088;
                auto* point = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        &path, nullptr, static_cast<int>(index)));
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                    trajectoryVtable[0x18 / 4])(trajectory, nullptr, point, time);
            }
        }
    }
    else
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            CSF_AT(void**, trajectory, 0)[0x18 / 4])(
                trajectory, nullptr,
                reinterpret_cast<float*>(&self->field_008c), 0.0f);
        const float endTime = self->field_0088;
        reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
            CSF_AT(void**, trajectory, 0)[0x18 / 4])(
                trajectory, nullptr,
                reinterpret_cast<float*>(&self->field_0098), endTime);
    }

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    alignas(8) uint8_t orderBytes[0x24];
    CSF_AT(void**, orderBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, orderBytes, 0x0C) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    std::memset(orderBytes + 0x14, 0, 12);
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;
    CSF_AT(uint32_t, orderBytes, 0x00) = 1;
    CSF_AT(uint32_t, orderBytes, 0x04) = 0;
    C_00833708_SendOrder(
        reinterpret_cast<C_00833708*>(owner->field_0044), nullptr,
        0, reinterpret_cast<int>(orderBytes));

    return C_0086c12c_ReleaseOwnedBuffer(
        reinterpret_cast<C_0086c12c*>(&path), nullptr);
}

HOOK(0x004D5940, C_0083ac40_BuildMovementPath, "auto");
