#include <cstdint>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_00405180_TraceGroundWithState, 0x004053A0, (S_00405180* self, void* , float* a2, float* a3, int* a4));
CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, IsNotReservedConfigCode, 0x00755A10, (int code));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static char __fastcall C_0082e090_TraceGroundAndScene(
    C_0082e090* self, void* /*edx*/, float* displacement, uint32_t* output)
{
    int traceState;
    char groundResult;

    if (self->field_0074)
    {
        std::memcpy(output, &self->field_0010, 0x64);
        self->field_0074 = 0;
        groundResult = 1;
    }
    else
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 1;
        traceState = 2;
        groundResult = S_00405180_TraceGroundWithState(
            reinterpret_cast<S_00405180*>(self->field_0008), nullptr,
            displacement, reinterpret_cast<float*>(output), &traceState);
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 0;
    }

    void* sceneController = reinterpret_cast<void*>(SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* sceneQuery =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0)[0x34 / 4])(
                sceneController, nullptr);

    float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr);
    float endX = position[0] + displacement[0];
    float endY = position[1] + displacement[1];
    float endZ = position[2] + displacement[2];

    alignas(8) uint8_t segmentBytes[0x18];
    position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr);
    CSF_AT(uint32_t, segmentBytes, 0x0) = CSF_AT(uint32_t, position, 0x0);
    CSF_AT(uint32_t, segmentBytes, 0x4) = CSF_AT(uint32_t, position, 0x4);
    CSF_AT(uint32_t, segmentBytes, 0x8) = CSF_AT(uint32_t, position, 0x8);
    CSF_AT(float, segmentBytes, 0x14) = endZ;
    CSF_AT(float, segmentBytes, 0xC) = endX;
    CSF_AT(float, segmentBytes, 0x10) = endY;

    traceState = 2;
    void* hitObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int, void*, int*, int)>(
            CSF_AT(void**, sceneQuery, 0)[0x34 / 4])(
                sceneQuery, nullptr, 0, segmentBytes, &traceState, 8);
    if (!hitObject)
        return groundResult;

    // Keep the contact point, normal, and hit metadata contiguous.
    alignas(8) uint8_t hitBytes[0x78];
    CSF_AT(uint8_t, hitBytes, 0x30) = 0xFF;
    CSF_AT(uint8_t, hitBytes, 0x20) = 0xFF;
    CSF_AT(int, hitBytes, 0x2C) = -1;
    CSF_AT(int, hitBytes, 0x1C) = -1;
    CSF_AT(float, hitBytes, 0x4) = 0.0f;
    CSF_AT(float, hitBytes, 0x8) = 0.0f;
    CSF_AT(float, hitBytes, 0xC) = 0.0f;
    CSF_AT(float, hitBytes, 0x10) = 0.0f;
    CSF_AT(float, hitBytes, 0x14) = 0.0f;
    CSF_AT(float, hitBytes, 0x18) = 0.0f;
    CSF_AT(int, hitBytes, 0x5C) = 0;
    CSF_AT(uint8_t, hitBytes, 0x28) = 0;
    CSF_AT(int, hitBytes, 0x60) = 0;
    CSF_AT(int, hitBytes, 0x64) = 10;
    CSF_AT(float, hitBytes, 0x0) = FLT_MAX;
    CSF_AT(uint32_t, hitBytes, 0x24) = 0;
    CSF_AT(C_0082e090*, hitBytes, 0x68) = self;
    CSF_AT(float, hitBytes, 0x6C) = endX;
    CSF_AT(float, hitBytes, 0x70) = endY;
    CSF_AT(float, hitBytes, 0x74) = endZ;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, hitObject, 0)[0xC / 4])(
            hitObject, nullptr, hitBytes);

    void* attachment = reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(
        CSF_AT(int, hitBytes, 0x24)));

    alignas(8) uint8_t directionBytes[0xC];
    auto* directionOutput = reinterpret_cast<uint32_t*>(directionBytes);

    if (static_cast<uint8_t>(ObjectAttachment_HasSoundTrigger(attachment, nullptr)) &&
        static_cast<uint8_t>(IsNotReservedConfigCode(
            CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x50))))
    {
        alignas(8) uint8_t recordBytes[0x74];
        C_0082e0f4& record = *reinterpret_cast<C_0082e0f4*>(recordBytes);
        C_0082e0f4_InitializeSpawnRecord(&record, nullptr, 4);

        void* owner = reinterpret_cast<void*>(self->field_0004);
        record.field_000c = CSF_AT(uint32_t, owner, 0x18C);
        record.field_0010 = CSF_AT(uint32_t, owner, 0x190);

        uint32_t* direction = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(&self->field_0084),
            nullptr, directionOutput);
        record.field_004c = direction[0];
        record.field_0050 = direction[1];
        record.field_0054 = direction[2];
        record.field_0034 = CSF_AT(uint32_t, hitBytes, 0x4);
        record.field_0038 = CSF_AT(uint32_t, hitBytes, 0x8);
        record.field_003c = CSF_AT(uint32_t, hitBytes, 0xC);
        record.field_0040 = CSF_AT(uint32_t, hitBytes, 0x10);
        record.field_0044 = CSF_AT(uint32_t, hitBytes, 0x14);
        record.field_0048 = CSF_AT(uint32_t, hitBytes, 0x18);
        record.field_0058 = CSF_AT(uint32_t, hitBytes, 0x24);

        void* resolver = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
        int context = CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x908);
        record.field_005c =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resolver, 0)[0x10 / 4])(
                    resolver, nullptr, context);

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);
        return groundResult;
    }

    output[0] = 1;
    CSF_AT(float, output, 0x8) =
        CSF_AT(float, hitBytes, 0x4) + CSF_AT(float, hitBytes, 0x10) * g_flt_0082DC84;
    CSF_AT(float, output, 0xC) =
        CSF_AT(float, hitBytes, 0x8) + CSF_AT(float, hitBytes, 0x14) * g_flt_0082DC84;
    CSF_AT(float, output, 0x10) =
        CSF_AT(float, hitBytes, 0xC) + CSF_AT(float, hitBytes, 0x18) * g_flt_0082DC84;

    std::memcpy(&output[5], &g_ZeroVectorX, sizeof(float));
    std::memcpy(&output[6], &g_ZeroVectorY, sizeof(float));
    std::memcpy(&output[7], &g_ZeroVectorZ, sizeof(float));

    output[22] = CSF_AT(uint32_t, hitBytes, 0x4);
    output[23] = CSF_AT(uint32_t, hitBytes, 0x8);
    output[24] = CSF_AT(uint32_t, hitBytes, 0xC);
    output[8] = CSF_AT(uint32_t, hitBytes, 0x4);
    output[9] = CSF_AT(uint32_t, hitBytes, 0x8);
    output[10] = CSF_AT(uint32_t, hitBytes, 0xC);

    uint32_t* direction = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(hitBytes + 0x10),
        nullptr, directionOutput);
    output[11] = direction[0];
    output[12] = direction[1];
    int surfaceId = CSF_AT(int, hitBytes, 0x1C);
    output[13] = direction[2];
    uint8_t surfaceFlags = CSF_AT(uint8_t, hitBytes, 0x20);
    CSF_AT(uint8_t, output, 0x48) = 1;
    output[19] = 0;
    output[20] = surfaceId;
    CSF_AT(uint8_t, output, 0x54) = surfaceFlags;
    return 1;
}

HOOK(0x00416650, C_0082e090_TraceGroundAndScene, "auto");
