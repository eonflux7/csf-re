#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_AimPrimaryEntryAtPosition, 0x0040DAB0, (void* self, void* , float* position, char interpolate));
CSF_FUNC(char, __fastcall, C_0083ac40_AimVehicleForwardAndSendOrder, 0x004CDF30, (void* self, void* edx));

static char __fastcall C_0083ac40_UpdateVehicleAim(
    C_0083ac40* self, void* /*edx*/, float deltaTime)
{
    const float elapsed = deltaTime + self->field_0084;
    void** curveVtable = reinterpret_cast<void**>(self->field_00a4);
    void* vehicle = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x4C);

    self->field_0084 = elapsed;

    alignas(8) uint8_t positionBytes[0xC];

    // The curve's time argument is passed as one raw stack dword.
    if (elapsed < self->field_0088)
    {
        uint32_t timeArgument;
        std::memcpy(&timeArgument, &elapsed, sizeof(timeArgument));

        reinterpret_cast<void(__fastcall*)(void*, void*, float*, uint32_t)>(
            curveVtable[0x14 / 4])(
                &self->field_00a4, nullptr,
                reinterpret_cast<float*>(positionBytes), timeArgument);

        return Vehicle_AimPrimaryEntryAtPosition(
            vehicle, nullptr, reinterpret_cast<float*>(positionBytes), 0);
    }

    uint32_t timeArgument;
    std::memcpy(&timeArgument, &self->field_0088, sizeof(timeArgument));

    reinterpret_cast<void(__fastcall*)(void*, void*, float*, uint32_t)>(
        curveVtable[0x14 / 4])(
            &self->field_00a4, nullptr,
            reinterpret_cast<float*>(positionBytes), timeArgument);

    Vehicle_AimPrimaryEntryAtPosition(
        vehicle, nullptr, reinterpret_cast<float*>(positionBytes), 0);

    switch (self->field_0070)
    {
    case 0:
    {
        void* controller = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0014), 0x44);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0xBC / 4])(
                controller, nullptr, 14);
        break;
    }
    case 1:
    {
        C_0083b958* task = CSF_AT(
            C_0083b958*, reinterpret_cast<void*>(self->field_0014), 0x30);
        task->vSlotA0();
        break;
    }
    }

    self->field_0070 = 4;
    return C_0083ac40_AimVehicleForwardAndSendOrder(self, nullptr);
}

HOOK(0x004CE460, C_0083ac40_UpdateVehicleAim, "auto");
