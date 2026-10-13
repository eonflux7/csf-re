#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008342e8_GetDrawState, 0x0049CF10, (C_008342e8* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));

static int __fastcall C_0082e090_DispatchSpawnRecord(
    C_0082e090* self, void* /*edx*/, uint32_t* parameters, int requestId)
{
    alignas(8) uint8_t recordBytes[0x74];
    auto& record = *reinterpret_cast<C_0082e0f4*>(recordBytes);

    alignas(8) uint8_t handleBytes[0x2C];
    alignas(8) uint8_t directionBytes[0xC];

    CSF_AT(uint32_t, handleBytes, 0x04) = -1;
    CSF_AT(uint32_t, handleBytes, 0x08) = -1;
    CSF_AT(uint32_t, handleBytes, 0x0C) = -1;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    CSF_AT(void**, handleBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, handleBytes, 0x18) = 0;
    CSF_AT(uint32_t, handleBytes, 0x1C) = 0;
    CSF_AT(uint32_t, handleBytes, 0x20) = 0;
    CSF_AT(uint32_t, handleBytes, 0x10) = 0;
    CSF_AT(uint32_t, handleBytes, 0x14) = 0;
    CSF_AT(uint32_t, handleBytes, 0x24) = 0;
    CSF_AT(uint32_t, handleBytes, 0x28) = 0;

    CSF_AT(uint32_t, handleBytes, 0x04) = CSF_AT(uint32_t, owner, 0x18C);
    CSF_AT(uint32_t, handleBytes, 0x08) = CSF_AT(uint32_t, owner, 0x190);
    CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(handleBytes));

    int drawState = C_008342e8_GetDrawState(
        reinterpret_cast<C_008342e8*>(self->field_0004), nullptr);
    CSF_AT(uint32_t, handleBytes, 0x0C) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(drawState), 0x70);

    drawState = C_008342e8_GetDrawState(
        reinterpret_cast<C_008342e8*>(self->field_0004), nullptr);
    CSF_AT(uint32_t, handleBytes, 0x10) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(drawState), 0x114);

    drawState = C_008342e8_GetDrawState(
        reinterpret_cast<C_008342e8*>(self->field_0004), nullptr);
    uint32_t drawFlags =
        CSF_AT(uint32_t, reinterpret_cast<void*>(drawState), 0x114);

    void* receiver = reinterpret_cast<void*>(parameters[14]);
    CSF_AT(int, handleBytes, 0x14) = requestId;
    CSF_AT(uint32_t, handleBytes, 0x28) = drawFlags;
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, receiver, 0)[0x8C / 4])(
            receiver, nullptr, handleBytes);

    C_0082e0f4_InitializeSpawnRecord(&record, nullptr, 4);

    uint32_t parameter8 = parameters[8];
    record.field_000c = CSF_AT(uint32_t, handleBytes, 0x04);
    uint32_t parameter9 = parameters[9];
    record.field_0010 = CSF_AT(uint32_t, handleBytes, 0x08);
    record.field_001c = parameter9;
    record.field_0018 = parameter8;

    uint32_t* convertedDirection = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(&self->field_0084),
        nullptr, reinterpret_cast<uint32_t*>(directionBytes));
    record.field_004c = convertedDirection[0];
    record.field_0050 = convertedDirection[1];
    record.field_0054 = convertedDirection[2];

    uint32_t* position = GetObjectPositionVector(owner, nullptr);
    record.field_0034 = position[0];
    record.field_0038 = position[1];
    record.field_003c = position[2];

    float* reverseDirection = reinterpret_cast<float*>(directionBytes);
    std::memcpy(&reverseDirection[0], &record.field_004c, sizeof(float));
    std::memcpy(&reverseDirection[1], &record.field_0050, sizeof(float));
    std::memcpy(&reverseDirection[2], &record.field_0054, sizeof(float));
    reverseDirection[0] = -reverseDirection[0];
    reverseDirection[1] = -reverseDirection[1];
    reverseDirection[2] = -reverseDirection[2];
    std::memcpy(&record.field_0040, &reverseDirection[0], sizeof(float));
    std::memcpy(&record.field_0044, &reverseDirection[1], sizeof(float));
    std::memcpy(&record.field_0048, &reverseDirection[2], sizeof(float));

    record.field_0024 = CSF_AT(uint32_t, owner, 0x194);
    record.field_0060 = CSF_AT(uint32_t, handleBytes, 0x0C);
    record.field_006c = CSF_AT(uint32_t, handleBytes, 0x10);
    *reinterpret_cast<uint8_t*>(&record.field_0070) = 1;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1));
}

HOOK(0x00416EA0, C_0082e090_DispatchSpawnRecord, "auto");
