#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_008342e8_CheckDepletionTarget, 0x0049D0A0, (void* self, void* edx, int a2, uint32_t* arg4));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));

static uint8_t __fastcall C_008342e8_NotifyDepleted(
    C_008342e8* self, void* /*edx*/, uint32_t* position, int reason)
{
    uint8_t result = CSF_AT(uint8_t, self, 0x184);
    if (result)
        return result;

    self->field_019c = position[0];
    CSF_AT(uint32_t, self, 0x1A0) = position[1];
    CSF_AT(uint32_t, self, 0x1A4) = position[2];

    alignas(8) uint8_t targetIndexBytes[0x4];
    auto* targetIndex = reinterpret_cast<uint32_t*>(targetIndexBytes);
    *targetIndex = static_cast<uint32_t>(-1);

    result = static_cast<uint8_t>(
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0xA8 / 4])(self, nullptr, reason));

    if (self->field_0050 != 9)
        return result;

    void* selectedTarget = nullptr;

    if (CSF_AT(int, self, 0x1B0) < 0)
    {
        float sourceRadius = 0.0f;
        void* sourceOwner = reinterpret_cast<void*>(self->field_004c);
        if (sourceOwner)
        {
            void* sourceObject =
                C_0082dd78_DispatchReturnedObject(sourceOwner, nullptr);
            if (sourceObject)
            {
                float* extent =
                    reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, sourceObject, 0)[0x30 / 4])(
                            sourceObject, nullptr);
                sourceRadius = std::sqrt(
                    extent[0] * extent[0] +
                    extent[1] * extent[1] +
                    extent[2] * extent[2]);
            }
        }

        GetObjectPositionVector(self, nullptr);

        alignas(8) uint8_t transformBytes[0x3C];
        alignas(8) uint8_t facingBytes[0xC];
        alignas(8) uint8_t matrixBytes[0x10];
        alignas(8) uint8_t sphereBytes[0x20];

        uint32_t sceneIndex = 0;
        for (;;)
        {
            result = static_cast<uint8_t>(sceneIndex);
            if (sceneIndex >= CSF_AT(uint32_t, g_pCurrentScene, 0x28))
                break;

            void* candidate = reinterpret_cast<void*>(
                CSF_AT(uint32_t*, g_pCurrentScene, 0x20)[sceneIndex]);

            if (candidate && CSF_AT(uint32_t, candidate, 0x50) == 4)
            {
                void* owner = CSF_AT(void*, candidate, 0x4C);
                void* object = nullptr;
                void* bounds = nullptr;
                if (owner)
                {
                    object = C_0082dd78_DispatchReturnedObject(owner, nullptr);
                    if (object)
                    {
                        bounds =
                            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, object, 0)[0x34 / 4])(
                                    object, nullptr);
                    }
                }

                if (owner && object && bounds)
                {
                    CSF_AT(uint32_t, sphereBytes, 0x00) = 0;
                    CSF_AT(uint32_t, sphereBytes, 0x04) = 0;
                    CSF_AT(uint32_t, sphereBytes, 0x08) = 0;
                    CSF_AT(float, sphereBytes, 0x0C) = 0.0f;

                    float* matrix =
                        reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                            CSF_AT(void**, bounds, 0)[0x1C / 4])(
                                bounds, nullptr,
                                reinterpret_cast<float*>(matrixBytes));
                    GfxMatrix_GetBoundingSphere(
                        matrix, nullptr, reinterpret_cast<float*>(sphereBytes));

                    float* targetPosition =
                        reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                            CSF_AT(void**, bounds, 0)[0x1C / 4])(
                                bounds, nullptr,
                                reinterpret_cast<float*>(transformBytes));

                    float targetRadius = CSF_AT(float, sphereBytes, 0x0C);
                    float* sourcePosition =
                        GetObjectPositionVector(self, nullptr);
                    float* facing = BuildFacingVectorBetween(
                        sourcePosition, nullptr,
                        reinterpret_cast<float*>(facingBytes), targetPosition);
                    long double distance = Vector_GetLength(facing, nullptr);

                    if (distance - sourceRadius < targetRadius &&
                        C_008342e8_CheckDepletionTarget(
                            self, nullptr, reinterpret_cast<int>(object),
                            targetIndex))
                    {
                        selectedTarget = candidate;
                    }
                }
            }

            ++sceneIndex;
            result = static_cast<uint8_t>(sceneIndex);
            if (selectedTarget)
                break;
        }
    }
    else
    {
        int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_01a8));
        result = static_cast<uint8_t>(resolved);
        void* target = reinterpret_cast<void*>(resolved);
        if (!target || CSF_AT(uint32_t, target, 0x50) != 4)
            return result;

        selectedTarget = target;
        *targetIndex = CSF_AT(uint32_t, self, 0x1B4);
    }

    if (!selectedTarget)
        return result;

    alignas(8) uint8_t notificationBytes[0x2C];
    CSF_AT(uint32_t, notificationBytes, 0x00) =
        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    CSF_AT(uint32_t, notificationBytes, 0x04) = self->field_0020;
    CSF_AT(uint32_t, notificationBytes, 0x08) = self->field_0024;
    CSF_AT(uint32_t, notificationBytes, 0x18) = 0;
    CSF_AT(uint32_t, notificationBytes, 0x1C) = 0;
    CSF_AT(uint32_t, notificationBytes, 0x20) = 0;
    CSF_AT(int, notificationBytes, 0x0C) = -1;
    CSF_AT(uint32_t, notificationBytes, 0x10) = 0;
    CSF_AT(uint32_t, notificationBytes, 0x14) = 0;
    CSF_AT(uint32_t, notificationBytes, 0x24) = 0;
    CSF_AT(uint32_t, notificationBytes, 0x28) = 0;

    int manager = GetValue8B0554();
    void* definitions = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x1FC0);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, definitions, 0)[0x08 / 4])(
                definitions, nullptr, CSF_AT(uint32_t, self, 0x194));
    CSF_AT(uint32_t, notificationBytes, 0x0C) =
        CSF_AT(uint32_t, definition, 0x74);

    manager = GetValue8B0554();
    definitions = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x1FC0);
    definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, definitions, 0)[0x08 / 4])(
                definitions, nullptr, CSF_AT(uint32_t, self, 0x194));
    CSF_AT(uint32_t, notificationBytes, 0x10) =
        CSF_AT(uint32_t, definition, 0x118);

    manager = GetValue8B0554();
    definitions = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x1FC0);
    definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, definitions, 0)[0x08 / 4])(
                definitions, nullptr, CSF_AT(uint32_t, self, 0x194));
    CSF_AT(uint32_t, notificationBytes, 0x28) =
        CSF_AT(uint32_t, definition, 0x118);

    void* recipient = CSF_AT(void*, selectedTarget, 0x38);
    CSF_AT(uint32_t, notificationBytes, 0x14) = *targetIndex;
    return static_cast<uint8_t>(
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, recipient, 0)[0x8C / 4])(
                recipient, nullptr,
                reinterpret_cast<uint32_t*>(notificationBytes)));
}

HOOK(0x0049D400, C_008342e8_NotifyDepleted, "auto");
