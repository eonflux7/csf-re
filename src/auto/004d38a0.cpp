#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d844_GetAttachedObjectField18, 0x004092A0, (C_0082d844* self, void*));
CSF_FUNC(char*, __fastcall, C_0082d844_GetPosition, 0x00409460, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, GfxTransform_GetOtherPosition, 0x00655FF0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(bool, __fastcall, GfxMatrix_ForwardValueWithAddress, 0x006ACA20, (void* self, void* , int a2));
CSF_FUNC(int, __fastcall, GfxMatrix_ClassifyPointAgainstPlane, 0x006AD7B0, (float* self, void* , float* point));
CSF_FUNC(double, __fastcall, GfxObjectProps_GetAttachmentHeight, 0x006B8360, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083BC04);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083bc28_ApplyDistanceScaledSceneOffsets(
    C_0083bc28* self, void* /*edx*/, int /*unused*/, float* plane)
{
    alignas(8) uint8_t targetMatrixBytes[0x3C];
    alignas(8) uint8_t objectMatrixBytes[0x3C];
    alignas(8) uint8_t offsetBytes[0xC];
    alignas(8) uint8_t sphereBytes[0x30];

    auto* sphere = reinterpret_cast<float*>(sphereBytes);
    auto* offset = reinterpret_cast<float*>(offsetBytes);
    sphere[0] = 0.0f;
    sphere[1] = 0.0f;
    sphere[2] = 0.0f;
    sphere[3] = 0.0f;

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    auto* skin = reinterpret_cast<C_0082dd78*>(owner->field_004c);
    auto* target =
        reinterpret_cast<C_0082d844*(__fastcall*)(void*, void*)>(
            skin->vtbl[0x3C / 4])(skin, nullptr);

    owner = reinterpret_cast<C_00833908*>(self->field_0014);
    void* returnedObject = C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<void*>(owner->field_004c), nullptr);
    void* matrixObject =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, returnedObject, 0)[0x34 / 4])(
                returnedObject, nullptr);
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, matrixObject, 0)[0x1C / 4])(
                matrixObject, nullptr, targetMatrixBytes);

    if (!target)
        return result;

    result = C_0082d844_GetAttachedObjectField18(target, nullptr);
    if (result == 1)
        return result;

    auto* scene = static_cast<uint8_t*>(g_pCurrentScene);
    uint32_t count = CSF_AT(uint32_t, scene, 0x68);
    auto* sceneList = scene + 0x60;
    result = static_cast<int>(count);
    if (count == 0)
        return result;

    uint32_t index = 0;
    do
    {
        const uint32_t entryOffset = 4 * index;
        auto* object = CSF_AT(C_00833908**, sceneList, 0)[index];
        float* position = GetObjectPositionVector(object, nullptr);

        if (!GfxMatrix_ClassifyPointAgainstPlane(plane, nullptr, position))
        {
            object = CSF_AT(C_00833908**, sceneList, 0)[index];
            void* transform = C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<void*>(object->field_004c), nullptr);
            float* dimensions =
                reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, transform, 0)[0x30 / 4])(
                        transform, nullptr);
            const float width = dimensions[0];
            const float depth = dimensions[2];

            matrixObject =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, transform, 0)[0x34 / 4])(
                        transform, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, matrixObject, 0)[0x1C / 4])(
                    matrixObject, nullptr, objectMatrixBytes);

            GfxMatrix_GetBoundingSphere(
                reinterpret_cast<float*>(objectMatrixBytes), nullptr, sphere);
            sphere[3] = width < depth ? depth : width;

            if (GfxMatrix_ForwardValueWithAddress(
                    targetMatrixBytes, nullptr, reinterpret_cast<int>(sphere)))
            {
                const float angularScale = static_cast<float>(
                    GfxObjectProps_GetAttachmentHeight(target, nullptr) *
                    g_DegreesToRadians);
                float* origin = reinterpret_cast<float*>(
                    GfxTransform_GetOtherPosition(target, nullptr));
                float* endpoint = reinterpret_cast<float*>(
                    C_0082d844_GetPosition(target, nullptr));
                const float dx = endpoint[0] - origin[0];
                const float dy = endpoint[1] - origin[1];
                const float dz = endpoint[2] - origin[2];

                auto** entry = reinterpret_cast<C_00833908**>(
                    CSF_AT(uint8_t*, sceneList, 0) + entryOffset);
                const float referenceDistance =
                    std::sqrt(dz * dz + dy * dy + dx * dx);

                origin = reinterpret_cast<float*>(
                    GfxTransform_GetOtherPosition(target, nullptr));
                position = GetObjectPositionVector(*entry, nullptr);
                const float objectDx = position[0] - origin[0];
                const float objectDy = position[1] - origin[1];
                const float objectDz = position[2] - origin[2];
                float ratio = std::sqrt(
                    objectDz * objectDz + objectDy * objectDy +
                    objectDx * objectDx) / referenceDistance;
                if (ratio > g_TransitionOne)
                    ratio = g_TransitionOne;
                const float magnitude = ratio * referenceDistance * angularScale;

                object = CSF_AT(C_00833908**, sceneList, 0)[index];
                void* skinObject = reinterpret_cast<void*>(object->field_004c);
                offset[0] = magnitude * plane[3];
                offset[1] = magnitude * plane[4];
                offset[2] = magnitude * plane[5];
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, float*, int)>(
                    CSF_AT(void**, skinObject, 0)[0x54 / 4])(
                        skinObject, nullptr, offset, &g_AxisY, 0);
            }
        }

        count = CSF_AT(uint32_t, sceneList, 0x8);
        result = static_cast<int>(++index);
    } while (index < count);

    return result;
}

HOOK(0x004D38A0, C_0083bc28_ApplyDistanceScaledSceneOffsets, "auto");
