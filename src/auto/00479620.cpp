#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeEventRecord, 0x004747B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(float*, __cdecl, WeaponFirearms_CalculateDirection, 0x00478930, (float* a1, float* a2, float* a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_SetObjectDirection, 0x0049CF40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static int __fastcall C_008317c8_SetResolvedObjectDirection(
    C_008317c8* self, void* /*edx*/, float* targetPosition)
{
    int result = static_cast<int>(self->field_006c);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
    if (!result)
        return result;

    void* object = reinterpret_cast<void*>(result);
    self->vSlotE8();

    float position[3];
    float target[3];
    float direction[3] = {};
    std::memcpy(
        position,
        reinterpret_cast<void*>(GetObjectPositionVector(object, nullptr)),
        sizeof(position));
    std::memcpy(target, targetPosition, sizeof(target));

    WeaponFirearms_CalculateDirection(direction, position, target);
    WeaponFirearms_SetObjectDirection(
        object, nullptr, reinterpret_cast<int>(direction));

    result = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        return result;

    C_00831658 record{};
    WeaponFirearms_InitializeEventRecord(&record, nullptr, 17);

    void* ownerData = reinterpret_cast<void*>(self->field_0004);
    record.field_000c = CSF_AT(uint32_t, ownerData, 0x20);
    record.field_0010 = CSF_AT(uint32_t, ownerData, 0x24);
    record.field_0024 =
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0x74);

    const float* objectPosition = reinterpret_cast<const float*>(
        GetObjectPositionVector(object, nullptr));
    std::memcpy(&record.field_0034, objectPosition, sizeof(float));
    std::memcpy(&record.field_0038, objectPosition + 1, sizeof(float));
    std::memcpy(&record.field_003c, objectPosition + 2, sizeof(float));
    std::memcpy(&record.field_0040, direction, sizeof(float));
    std::memcpy(&record.field_0044, direction + 1, sizeof(float));
    std::memcpy(&record.field_0048, direction + 2, sizeof(float));
    record.field_004c = reinterpret_cast<uint32_t>(object);

    void* manager = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0));
    void* valuePointer =
        reinterpret_cast<void*>(Handle_GetValuePointer(self, nullptr));
    uint32_t handleValue = CSF_AT(uint32_t, valuePointer, 0);
    int resolvedValue =
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, manager, 0)[0xC / 4])(
                manager, nullptr, handleValue);

    std::memcpy(&record.field_0054, direction, sizeof(float));
    record.field_0050 = static_cast<uint32_t>(resolvedValue);
    std::memcpy(&record.field_0058, direction + 1, sizeof(float));
    std::memcpy(&record.field_005c, direction + 2, sizeof(float));

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1));
}

HOOK(0x00479620, C_008317c8_SetResolvedObjectDirection, "auto");
