#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnRecord, 0x004164B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeAreaEffectRecord, 0x00446550, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453660, 0x00453660, (CGuiControl* self, void* , S_0041e5f0* a2, float* a3, int* a4, float a5));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SubmitSceneEffect, 0x0048B280, (void* self, void* edx, float* arg0));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t Effects_FloatStorage(float value)
{
    uint32_t storage;
    std::memcpy(&storage, &value, sizeof(storage));
    return storage;
}

static int __stdcall Effects_ApplyAreaEffect(void* effect, float scale)
{
    // The record at -0xB8 includes the descriptor starting at -0x8C.
    alignas(8) uint8_t areaRecordBytes[0x60];
    C_008310f8& areaRecord =
        *reinterpret_cast<C_008310f8*>(areaRecordBytes);

    if (CSF_AT(int32_t, effect, 0x00) == 0)
    {
        alignas(8) uint8_t sceneEffectBytes[0x34];

        const float originX = CSF_AT(float, effect, 0x04);
        const float raisedY =
            CSF_AT(float, effect, 0x08) + g_TransitionOne;
        const float originZ = CSF_AT(float, effect, 0x0C);
        const uint32_t amount =
            static_cast<uint32_t>(
                static_cast<int64_t>(CSF_AT(float, effect, 0x18)));

        CSF_AT(float, sceneEffectBytes, 0x00) = 0.0f;
        CSF_AT(float, sceneEffectBytes, 0x04) = originX;
        CSF_AT(float, sceneEffectBytes, 0x08) = raisedY;
        CSF_AT(float, sceneEffectBytes, 0x0C) = originZ;
        CSF_AT(uint32_t, sceneEffectBytes, 0x10) = 0; // Uninitialized in the original.
        CSF_AT(int32_t, sceneEffectBytes, 0x14) = -1;
        CSF_AT(uint32_t, sceneEffectBytes, 0x18) =
            CSF_AT(uint32_t, effect, 0x10);
        CSF_AT(uint32_t, sceneEffectBytes, 0x1C) = 0;
        CSF_AT(uint32_t, sceneEffectBytes, 0x20) = amount;
        CSF_AT(uint32_t, sceneEffectBytes, 0x24) = amount;
        CSF_AT(float, sceneEffectBytes, 0x28) = originX;
        CSF_AT(float, sceneEffectBytes, 0x2C) = raisedY;
        CSF_AT(float, sceneEffectBytes, 0x30) = originZ;

        SubmitSceneEffect(
            CSF_AT(void*, g_pCurrentScene, 0xA8),
            nullptr, reinterpret_cast<float*>(sceneEffectBytes));

        InitializeAreaEffectRecord(&areaRecord, nullptr, 14);

        const float recordAmount = CSF_AT(float, effect, 0x18);
        areaRecord.field_0034 = CSF_AT(uint32_t, effect, 0x04);
        areaRecord.field_0038 = CSF_AT(uint32_t, effect, 0x08);
        areaRecord.field_003c = CSF_AT(uint32_t, effect, 0x0C);
        areaRecord.field_0048 = Effects_FloatStorage(g_AxisY);
        areaRecord.field_004c = Effects_FloatStorage(g_AxisY_Y);
        areaRecord.field_0050 = Effects_FloatStorage(g_AxisY_Z);
        areaRecord.field_0024 = -1;
        areaRecord.field_0028 = 0;
        areaRecord.field_0040 = 0;
        areaRecord.field_0044 = 0;
        areaRecord.field_0054 = CSF_AT(uint32_t, effect, 0x10);
        areaRecord.field_0058 =
            static_cast<uint32_t>(static_cast<int64_t>(recordAmount));

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        return static_cast<int>(
            DispatchRecordToGlobalObject(dispatcher, nullptr, &areaRecord, 1));
    }

    alignas(8) uint8_t boundsBytes[0x10];
    float* bounds = reinterpret_cast<float*>(boundsBytes);
    bounds[0] = CSF_AT(float, effect, 0x04);
    bounds[1] =
        CSF_AT(float, effect, 0x08) + g_fReturnedConstant_0082E030;
    bounds[2] = CSF_AT(float, effect, 0x0C);
    bounds[3] = CSF_AT(float, effect, 0x10);

    alignas(8) uint8_t objectListBytes[0x10];
    CSF_AT(uint32_t, objectListBytes, 0x00) = 0;
    CSF_AT(uint32_t, objectListBytes, 0x04) = 0;
    CSF_AT(uint32_t, objectListBytes, 0x08) = 0;
    CSF_AT(int32_t, objectListBytes, 0x0C) = 10;

    areaRecord.field_0048 = 10;
    std::memset(&areaRecord.field_0030, 255, 12);
    areaRecord.field_0058 = -1;
    std::memset(&areaRecord.field_003c, 0, 12);
    std::memset(&areaRecord.field_004c, 0, 12);
    std::memset(&areaRecord.field_005c, 0, 4);
    areaRecord.field_002c = 273;

    CGuiControl_ProcessDisplayFlags_453660(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        reinterpret_cast<S_0041e5f0*>(objectListBytes), bounds,
        reinterpret_cast<int*>(&areaRecord.field_002c), 0.0f);

    const uint32_t scaledAmount =
        static_cast<uint32_t>(
            static_cast<int64_t>(scale * CSF_AT(float, effect, 0x18)));
    const uint32_t objectCount = CSF_AT(uint32_t, objectListBytes, 0x08);
    uint8_t* origin = static_cast<uint8_t*>(effect) + 0x04;

    alignas(8) uint8_t notificationBytes[0x38];
    alignas(8) uint8_t spawnRecordBytes[0x74];
    C_0082e0f4& spawnRecord =
        *reinterpret_cast<C_0082e0f4*>(spawnRecordBytes);

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        void* object = CSF_AT(void*, objectListBytes, 0x00);
        object = static_cast<void**>(object)[index];

        CSF_AT(void**, notificationBytes, 0x00) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, notificationBytes, 0x04) = -1;
        CSF_AT(int32_t, notificationBytes, 0x08) = -1;
        CSF_AT(int32_t, notificationBytes, 0x0C) = -1;
        CSF_AT(uint32_t, notificationBytes, 0x10) = scaledAmount;
        CSF_AT(uint32_t, notificationBytes, 0x14) = 0;
        CSF_AT(uint32_t, notificationBytes, 0x18) = 0;
        CSF_AT(uint32_t, notificationBytes, 0x1C) = 0;
        CSF_AT(uint32_t, notificationBytes, 0x20) = 0;
        CSF_AT(int32_t, notificationBytes, 0x24) = 6;
        CSF_AT(uint32_t, notificationBytes, 0x28) = 0;
        CSF_AT(float, notificationBytes, 0x2C) = bounds[0];
        CSF_AT(float, notificationBytes, 0x30) = bounds[1];
        CSF_AT(float, notificationBytes, 0x34) = bounds[2];

        void* receiver = CSF_AT(void*, object, 0x38);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, receiver, 0x00)[0x8C / 4])(
                receiver, nullptr, notificationBytes);

        InitializeSpawnRecord(&spawnRecord, nullptr, 4);
        spawnRecord.field_0018 = CSF_AT(uint32_t, object, 0x20);
        spawnRecord.field_001c = CSF_AT(uint32_t, object, 0x24);
        spawnRecord.field_0034 = CSF_AT(uint32_t, origin, 0x00);
        spawnRecord.field_0038 = CSF_AT(uint32_t, origin, 0x04);
        spawnRecord.field_003c = CSF_AT(uint32_t, origin, 0x08);

        float* position = GetObjectPositionVector(object, nullptr);
        const float deltaX = position[0] - CSF_AT(float, origin, 0x00);
        const float deltaY = position[1] - CSF_AT(float, origin, 0x04);
        const float deltaZ = position[2] - CSF_AT(float, origin, 0x08);
        CSF_AT(float, notificationBytes, 0x2C) = deltaX;
        CSF_AT(float, notificationBytes, 0x30) = deltaY;
        CSF_AT(float, notificationBytes, 0x34) = deltaZ;
        spawnRecord.field_004c = Effects_FloatStorage(deltaX);
        spawnRecord.field_0050 = Effects_FloatStorage(deltaY);
        spawnRecord.field_0054 = Effects_FloatStorage(deltaZ);
        spawnRecord.field_0060 = -1;
        spawnRecord.field_006c = scaledAmount;

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, &spawnRecord, 1);

        spawnRecord.field_0014 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
        spawnRecord.field_0008 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
        spawnRecord.vtbl = CSF_VTBL(0x0082D410);
        CSF_AT(void**, notificationBytes, 0x00) = CSF_VTBL(0x0082D410);
    }

    if (areaRecord.field_003c)
    {
        Mem_Free(reinterpret_cast<void*>(areaRecord.field_003c));
        areaRecord.field_003c = 0;
    }

    void* listStorage = CSF_AT(void*, objectListBytes, 0x00);
    if (listStorage)
        return Mem_Free(listStorage);
    return 0;
}

HOOK(0x00446910, Effects_ApplyAreaEffect, "auto");
