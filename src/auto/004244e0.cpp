#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(bool, __fastcall, GfxMatrix_ForwardValueWithAddress, 0x006ACA20, (void* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Const_0082E068, 0x0082E068);
CSF_GLOBAL(float, g_MaxVehicleTimeStep, 0x0082E26C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall C_0082faf8_ApplyMovementVector(
    C_0082faf8* self, void* /*edx*/, S_00526d40* movement)
{
    alignas(8) uint8_t sourceBoundsBytes[0x3C];
    alignas(8) uint8_t targetBoundsBytes[0x3C];
    alignas(8) uint8_t convertedDirectionBytes[0xC];
    alignas(8) uint8_t spawnRecordBytes[0x74];
    alignas(8) uint8_t forwardBytes[0xC];
    alignas(8) uint8_t impulseBytes[0xC];
    alignas(8) uint8_t impactRecordBytes[0x2C];
    alignas(8) uint8_t sphereBytes[0x24];
    alignas(8) uint8_t directionBytes[0x10];

    auto& spawnRecord =
        *reinterpret_cast<C_0082e0f4*>(spawnRecordBytes);
    auto* forward = reinterpret_cast<float*>(forwardBytes);
    auto* impulse = reinterpret_cast<float*>(impulseBytes);
    auto* sphere = reinterpret_cast<float*>(sphereBytes);
    auto* direction = reinterpret_cast<float*>(directionBytes);

    auto* source = reinterpret_cast<C_008342e8*>(self->field_0014);
    void* boundsProvider =
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<void*>(source->field_004c), nullptr);
    void* boundsObject =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, boundsProvider, 0)[0x34 / 4])(
                boundsProvider, nullptr);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, boundsObject, 0)[0x1C / 4])(
            boundsObject, nullptr, sourceBoundsBytes);

    void* scene = g_pCurrentScene;
    const uint32_t initialCount = CSF_AT(uint32_t, scene, 0x68);
    auto* objectList = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(scene) + 0x60);

    if (initialCount == 0)
        return objectList;

    uint32_t index = 0;
    for (;;)
    {
        auto* target = reinterpret_cast<C_008342e8*>(
            CSF_AT(C_008342e8**, objectList, 0)[index]);
        C_0082dd78* targetBody =
            reinterpret_cast<C_0082dd78*>(target->field_004c);

        if (static_cast<uint8_t>(
                IsField1BCNonNegative(target, nullptr)) == 0 &&
            static_cast<uint8_t>(
                SceneObject_PassesFirstCheck(target, nullptr)) == 0 &&
            ((int (__fastcall*)(C_0082dd78*, void*))
                targetBody->vtbl[0x4C / 4])(targetBody, nullptr) == 0)
        {
            boundsProvider =
                C_0082dd78_DispatchReturnedObject(
                    reinterpret_cast<void*>(target->field_004c),
                    nullptr);
            boundsObject =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, boundsProvider, 0)[0x34 / 4])(
                        boundsProvider, nullptr);
            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, boundsObject, 0)[0x1C / 4])(
                    boundsObject, nullptr, targetBoundsBytes);

            std::memset(sphereBytes, 0, 3 * sizeof(float));
            sphere[3] = 0.0f;
            GfxMatrix_GetBoundingSphere(
                reinterpret_cast<float*>(targetBoundsBytes),
                nullptr, sphere);
            sphere[3] *= g_Const_0082E068;

            if (GfxMatrix_ForwardValueWithAddress(
                    sourceBoundsBytes, nullptr,
                    reinterpret_cast<int>(sphereBytes)))
            {
                source =
                    reinterpret_cast<C_008342e8*>(self->field_0014);
                void* motionProvider =
                    ((void* (__fastcall*)(void*, void*))
                        source->vtbl[0x28 / 4])(source, nullptr);
                void* motionObject =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, motionProvider, 0)[0x18 / 4])(
                            motionProvider, nullptr);
                ((void (__fastcall*)(void*, void*, float*))
                    CSF_AT(void**, motionObject, 0)[0x5C / 4])(
                        motionObject, nullptr, forward);

                float* sourcePosition = GetObjectPositionVector(
                    reinterpret_cast<void*>(self->field_0014), nullptr);
                float* targetPosition =
                    GetObjectPositionVector(target, nullptr);

                direction[2] = targetPosition[2] - sourcePosition[2];
                direction[1] = targetPosition[1] - sourcePosition[1];
                direction[0] = targetPosition[0] - sourcePosition[0];
                NormalizeVector3(
                    reinterpret_cast<C_00833b88*>(directionBytes),
                    nullptr);

                const float strength =
                    (forward[1] * direction[1] +
                     forward[0] * direction[0] +
                     forward[2] * direction[2]) * self->field_0050;

                if (strength > g_fZero)
                {
                    CSF_AT(uint32_t, impactRecordBytes, 0x04) = -1;
                    CSF_AT(uint32_t, impactRecordBytes, 0x08) = -1;
                    CSF_AT(uint32_t, impactRecordBytes, 0x14) = -1;
                    CSF_AT(uint32_t, impactRecordBytes, 0x0C) = -1;

                    source =
                        reinterpret_cast<C_008342e8*>(self->field_0014);
                    CSF_AT(void**, impactRecordBytes, 0) =
                        CSF_VTBL(0x0082D448);
                    CSF_AT(uint32_t, impactRecordBytes, 0x18) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x1C) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x20) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x10) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x24) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x28) = 0;
                    CSF_AT(uint32_t, impactRecordBytes, 0x04) =
                        source->field_0020;
                    CSF_AT(uint32_t, impactRecordBytes, 0x08) =
                        source->field_0024;

                    void* settings = reinterpret_cast<void*>(
                        GetGlobalValue_8BC1E0());
                    CSF_AT(uint32_t, impactRecordBytes, 0x10) =
                        static_cast<uint32_t>(static_cast<int64_t>(
                            strength *
                            CSF_AT(float, settings, 0x93C) *
                            g_MaxVehicleTimeStep));

                    void* impactReceiver =
                        reinterpret_cast<void*>(target->field_0038);
                    ((void (__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, impactReceiver, 0)[0x8C / 4])(
                            impactReceiver, nullptr, impactRecordBytes);

                    C_0082e0f4_InitializeSpawnRecord(
                        &spawnRecord, nullptr, 4);
                    spawnRecord.field_000c =
                        CSF_AT(uint32_t, impactRecordBytes, 0x04);
                    spawnRecord.field_001c = target->field_0024;
                    spawnRecord.field_0010 =
                        CSF_AT(uint32_t, impactRecordBytes, 0x08);
                    spawnRecord.field_0018 = target->field_0020;
                    spawnRecord.field_0054 =
                        CSF_AT(uint32_t, directionBytes, 0x08);
                    spawnRecord.field_004c =
                        CSF_AT(uint32_t, directionBytes, 0x00);
                    spawnRecord.field_0050 =
                        CSF_AT(uint32_t, directionBytes, 0x04);

                    targetPosition =
                        GetObjectPositionVector(target, nullptr);
                    const uint32_t positionX =
                        CSF_AT(uint32_t, targetPosition, 0x00);
                    CSF_AT(float, sphereBytes, 0x10) =
                        -CSF_AT(float, &spawnRecord, 0x4C);
                    spawnRecord.field_0034 = positionX;
                    spawnRecord.field_0038 =
                        CSF_AT(uint32_t, targetPosition, 0x04);
                    const uint32_t positionZ =
                        CSF_AT(uint32_t, targetPosition, 0x08);
                    CSF_AT(float, sphereBytes, 0x14) =
                        -CSF_AT(float, &spawnRecord, 0x50);
                    spawnRecord.field_0044 =
                        CSF_AT(uint32_t, sphereBytes, 0x14);
                    CSF_AT(float, sphereBytes, 0x18) =
                        -CSF_AT(float, &spawnRecord, 0x54);
                    spawnRecord.field_003c = positionZ;
                    spawnRecord.field_0040 =
                        CSF_AT(uint32_t, sphereBytes, 0x10);
                    spawnRecord.field_0048 =
                        CSF_AT(uint32_t, sphereBytes, 0x18);
                    spawnRecord.field_0060 = -1;
                    spawnRecord.field_006c =
                        CSF_AT(uint32_t, impactRecordBytes, 0x10);
                    spawnRecord.field_0070 = 1;

                    void* dispatcher = reinterpret_cast<void*>(
                        GetGlobalObjectField184());
                    DispatchRecordToGlobalObject(
                        dispatcher, nullptr, &spawnRecord, 1);

                    settings = reinterpret_cast<void*>(
                        GetGlobalValue_8BC1E0());
                    // Snapshot the vtable before direction conversion.
                    auto* updateVtable = targetBody->vtbl;
                    const float impulseScale =
                        CSF_AT(float, settings, 0x938) *
                        g_HalfDurationScale * strength;
                    impulse[0] = direction[0] * impulseScale;
                    impulse[1] = direction[1] * impulseScale;
                    impulse[2] = direction[2] * impulseScale;

                    uint32_t* convertedDirection =
                        S_00526d40_ConvertDirection(
                            movement, nullptr,
                            reinterpret_cast<uint32_t*>(
                                convertedDirectionBytes));
                    ((void (__fastcall*)(
                        C_0082dd78*, void*, float*, uint32_t*, uint32_t))
                        updateVtable[0x54 / 4])(
                            targetBody, nullptr, impulse,
                            convertedDirection, 0);

                    auto* baseVtable = CSF_VTBL(0x0082D410);
                    spawnRecord.field_0014 =
                        reinterpret_cast<uint32_t>(baseVtable);
                    spawnRecord.field_0008 =
                        reinterpret_cast<uint32_t>(baseVtable);
                    spawnRecord.vtbl = baseVtable;
                    CSF_AT(void**, impactRecordBytes, 0) = baseVtable;
                }
            }
        }

        const uint32_t count = CSF_AT(uint32_t, objectList, 0x08);
        ++index;
        if (index >= count)
            return reinterpret_cast<uint32_t*>(index);
    }
}

HOOK(0x004244E0, C_0082faf8_ApplyMovementVector, "auto");
