#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);

// The packet also identifies the grow routine's address as a vtable reference.
[[maybe_unused]] static const auto g_EventRecordTable = CSF_VTBL(0x00413570);

static uint32_t* __fastcall C_0082dfd0_RecordNearbyObject(
    C_0082dfd0* self, void* /*edx*/, C_008342e8* object,
    uint8_t bypassDistanceCheck)
{
    if (!bypassDistanceCheck)
    {
        void* returnedObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(object->field_004c),
                nullptr));

        const float* dimensions =
            reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, returnedObject, 0)[0x30 / 4])(
                    returnedObject, nullptr);

        const float width = dimensions[0];
        const float depth = dimensions[2];
        const float scaledDepth = depth * g_ValueScale;
        const float scaledWidth = width * g_ValueScale;
        const float radius =
            (scaledWidth < scaledDepth ? scaledDepth : scaledWidth)
            * g_ScaleY_0_6;

        const float* objectPosition = reinterpret_cast<const float*>(
            GetObjectPositionVector(object, nullptr));
        const float objectX = objectPosition[0];
        const float objectZ = objectPosition[2];

        const float* ownerPosition = reinterpret_cast<const float*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr));

        const float dx = objectX - ownerPosition[0];
        const float dz = objectZ - ownerPosition[2];
        const float verticalDifference = g_fZero;
        const float distanceSquared =
            dz * dz + dx * dx + verticalDifference * verticalDifference;

        if (!(radius * radius > distanceSquared))
            return nullptr;
    }

    object->vSlot6C(0);

    if (self->field_0154 >= self->field_0150)
    {
        MissionScript_GrowEventRecords(
            reinterpret_cast<uint8_t*>(self) + 0x14C,
            nullptr, CSF_AT(int, self, 0x158));
    }

    const uint32_t recordIndex = self->field_0154;
    const uint32_t recordsAddress = self->field_014c;
    self->field_0154 = recordIndex + 1;

    void* record = reinterpret_cast<void*>(recordsAddress + 12 * recordIndex);
    CSF_AT(uint32_t, record, 0x4) = object->field_0020;
    CSF_AT(uint32_t, record, 0x8) = object->field_0024;

    return reinterpret_cast<uint32_t*>(object);
}

HOOK(0x00413C60, C_0082dfd0_RecordNearbyObject, "auto");
