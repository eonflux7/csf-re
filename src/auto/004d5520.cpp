#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_0041c550_IsField8CNull, 0x0041C550, (void* self, void*));
CSF_FUNC(bool, __fastcall, Vehicle_AimAtPosition, 0x0041C960, (void* self, void* , float* position, char interpolate));

// Reads the object linked at this+0x14, takes its field at +0x4C, and tests it with
// C_0041c550_IsField8CNull. If that is true, aims the vehicle at the float block at
// this+0x8C with interpolate = 1. Otherwise returns the IsField8CNull result unchanged.
static int __fastcall AimVehicleIfLinkedFieldPresent(void* self, void* /*edx*/) {
    uint32_t linkedPtr = CSF_AT(uint32_t, self, 0x14);
    void* linked = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedPtr));
    uint32_t fieldPtr = CSF_AT(uint32_t, linked, 0x4C);
    void* fieldObj = reinterpret_cast<void*>(static_cast<uintptr_t>(fieldPtr));

    int result = C_0041c550_IsField8CNull(fieldObj, nullptr);
    if ((uint8_t)result) {
        float* positionBlock = reinterpret_cast<float*>(static_cast<uint8_t*>(self) + 0x8C);
        return Vehicle_AimAtPosition(fieldObj, nullptr, positionBlock, 1) ? 1 : 0;
    }
    return result;
}

HOOK(0x004D5520, AimVehicleIfLinkedFieldPresent, "auto");
