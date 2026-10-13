#include <cstdint>
#include "globals.h"
#include "hook.h"
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

static int __fastcall C_00835228_BuildMovementPathAndSendOrder(
    C_00835228* self, void* /*edx*/)
{
    void* movementPath = &self->field_009c;
    void** movementVtable = reinterpret_cast<void**>(self->field_009c);
    self->field_0078 = 0.0f;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        movementVtable[0x1C / 4])(movementPath, nullptr);

    alignas(8) uint8_t pathBytes[0x14];
    auto* path = reinterpret_cast<C_0085e7b8*>(pathBytes);
    auto* pathBuffer = reinterpret_cast<C_0086c12c*>(pathBytes);
    alignas(8) uint8_t destinationValueBytes[0x18];
    auto* destinationValue = reinterpret_cast<uint32_t*>(destinationValueBytes);

    C_0086c12c_Construct(pathBuffer, nullptr);
    // The original loads this into ECX before the no-argument getter.
    void* currentScene = g_pCurrentScene;
    (void)currentScene;
    auto* pathFinder = reinterpret_cast<S_0075f7c0*>(GetGlobalNestedField14());
    if (S_0075f7c0_FindPath_75FC40(
            pathFinder, nullptr,
            reinterpret_cast<float*>(&self->field_0080),
            reinterpret_cast<float*>(&self->field_008c),
            path, destinationValue))
    {
        ForwardToInnerWithZeroFlag(path, nullptr, &self->field_0080);
        const uint32_t pointCount = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(GetCachedObjectProvider(path, nullptr)));

        float totalSquaredDistance = 0.0f;
        for (uint32_t index = 1; index < pointCount; ++index)
        {
            const float* previous = reinterpret_cast<float*>(
                C_0085e7b8_GetPathPointRecord(
                    path, nullptr, static_cast<int>(index - 1)));
            const float* current = reinterpret_cast<float*>(
                C_0085e7b8_GetPathPointRecord(
                    path, nullptr, static_cast<int>(index)));
            const float dx = current[0] - previous[0];
            const float dy = current[1] - previous[1];
            const float dz = current[2] - previous[2];
            totalSquaredDistance = dz * dz + dy * dy + dx * dx
                                 + totalSquaredDistance;
        }

        movementVtable = reinterpret_cast<void**>(self->field_009c);
        float accumulatedSquaredDistance = 0.0f;
        const float* firstPoint = reinterpret_cast<float*>(
            C_0085e7b8_GetPathPointRecord(path, nullptr, 0));
        reinterpret_cast<void(__fastcall*)(void*, void*, const float*, float)>(
            movementVtable[0x18 / 4])(movementPath, nullptr, firstPoint, 0.0f);

        if (pointCount > 1)
        {
            const float inverseSquaredDistance =
                g_TransitionOne / totalSquaredDistance;
            for (uint32_t index = 1; index < pointCount; ++index)
            {
                const float* previous = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        path, nullptr, static_cast<int>(index - 1)));
                const float* current = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        path, nullptr, static_cast<int>(index)));
                const float dx = current[0] - previous[0];
                const float dy = current[1] - previous[1];
                const float dz = current[2] - previous[2];

                movementVtable = reinterpret_cast<void**>(self->field_009c);
                accumulatedSquaredDistance = dz * dz + dy * dy + dx * dx
                                           + accumulatedSquaredDistance;
                const float time = inverseSquaredDistance
                                 * accumulatedSquaredDistance
                                 * self->field_007c;
                const float* point = reinterpret_cast<float*>(
                    C_0085e7b8_GetPathPointRecord(
                        path, nullptr, static_cast<int>(index)));
                reinterpret_cast<void(__fastcall*)(void*, void*, const float*, float)>(
                    movementVtable[0x18 / 4])(movementPath, nullptr, point, time);
            }
        }
    }
    else
    {
        movementVtable = reinterpret_cast<void**>(self->field_009c);
        reinterpret_cast<void(__fastcall*)(void*, void*, const float*, float)>(
            movementVtable[0x18 / 4])(
                movementPath, nullptr,
                reinterpret_cast<float*>(&self->field_0080), 0.0f);

        const float time = self->field_007c;
        movementVtable = reinterpret_cast<void**>(self->field_009c);
        reinterpret_cast<void(__fastcall*)(void*, void*, const float*, float)>(
            movementVtable[0x18 / 4])(
                movementPath, nullptr,
                reinterpret_cast<float*>(&self->field_008c), time);
    }

    alignas(8) uint8_t orderBytes[0x24];
    CSF_AT(uint32_t, orderBytes, 0x14) = self->field_0080;
    CSF_AT(int, orderBytes, 0x0C) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    CSF_AT(uint32_t, orderBytes, 0x18) = self->field_0084;
    const uint32_t originZ = self->field_0088;
    auto* character = reinterpret_cast<C_00833908*>(self->field_0014);

    CSF_AT(uint32_t, orderBytes, 0x00) = 1;
    CSF_AT(void**, orderBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;
    CSF_AT(uint32_t, orderBytes, 0x04) = 3;
    CSF_AT(uint32_t, orderBytes, 0x1C) = originZ;

    C_00833708_SendOrder(
        reinterpret_cast<C_00833708*>(character->field_0044),
        nullptr, 0, reinterpret_cast<int>(orderBytes));
    return C_0086c12c_ReleaseOwnedBuffer(pathBuffer, nullptr);
}

HOOK(0x004A9140, C_00835228_BuildMovementPathAndSendOrder, "auto");
