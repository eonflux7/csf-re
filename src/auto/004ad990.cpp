#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004ad990_ConfigureDestination(
    S_004ad990* self, void* /*edx*/, uint32_t* parameters)
{
    if (self->field_0038 != parameters[4])
        self->field_00a4 = 0;

    self->field_0028 = parameters[0];
    self->field_002c = parameters[1];
    self->field_0030 = parameters[2];
    self->field_0038 = parameters[4];
    self->field_003c = parameters[5];
    self->field_0040 = CSF_AT(float, parameters, 0x18);
    self->field_0044 = CSF_AT(float, parameters, 0x1C);
    self->field_0048 = CSF_AT(float, parameters, 0x20);
    CSF_AT(uint32_t, self, 0x50) = parameters[10];
    self->field_0054 = parameters[11];
    CSF_AT(uint32_t, self, 0x58) = parameters[12];

    float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    const float deltaX = self->field_0040 - position[0];
    const float deltaY = self->field_0044 - position[1];
    const float deltaZ = self->field_0048 - position[2];
    const int32_t targetIndex = static_cast<int32_t>(self->field_0054);

    self->field_009c = 0.0f;
    self->field_00a9 = 0;
    self->field_00a8 = 1;
    self->field_0098 =
        deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX;

    const int handleAddress = static_cast<int>(
        reinterpret_cast<intptr_t>(reinterpret_cast<uint8_t*>(self) + 0x4C));

    float* startPosition;
    float destinationZ;
    if (targetIndex >= 0 &&
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress))
    {
        const int target = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, handleAddress);
        float* targetPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(target), nullptr);
        startPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0000), nullptr);

        self->field_005c = 0;
        CSF_AT(float, self, 0x70) = targetPosition[0];
        CSF_AT(float, self, 0x74) = targetPosition[1];
        destinationZ = targetPosition[2];
    }
    else
    {
        startPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0000), nullptr);

        self->field_005c = 0;
        CSF_AT(float, self, 0x70) = self->field_0040;
        CSF_AT(float, self, 0x74) = self->field_0044;
        destinationZ = self->field_0048;
    }

    self->field_0060 = 1065353216;
    CSF_AT(float, self, 0x78) = destinationZ;

    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, startPosition, 0x00);
    CSF_AT(uint32_t, self, 0x68) = CSF_AT(uint32_t, startPosition, 0x04);
    const int result = CSF_AT(int, startPosition, 0x08);
    CSF_AT(int, self, 0x6C) = result;
    return result;
}

HOOK(0x004AD990, S_004ad990_ConfigureDestination, "auto");
