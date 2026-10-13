#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, IsField0050Null, 0x0040DA80, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, Vehicle_AimPrimaryEntryAtPosition, 0x0040DAB0, (void* self, void* edx, float* position, char interpolate));

// Returns the result of IsField0050Null on the object held at this->field_0014->field_004C.
// If that check is true, runs Vehicle_AimPrimaryEntryAtPosition on that same object,
// passing the float block at this+0x8C and flag 1.
static int __fastcall AimPrimaryEntryIfField0050Present(void* self, void* /*edx*/) {
    uint32_t linkedPtr = CSF_AT(uint32_t, self, 0x14);
    void* linked = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedPtr));
    uint32_t fieldPtr = CSF_AT(uint32_t, linked, 0x4C);
    void* fieldObj = reinterpret_cast<void*>(static_cast<uintptr_t>(fieldPtr));

    int result = IsField0050Null(fieldObj, nullptr);
    if ((uint8_t)result) {
        float* positionBlock = reinterpret_cast<float*>(static_cast<uint8_t*>(self) + 0x8C);
        return Vehicle_AimPrimaryEntryAtPosition(fieldObj, nullptr, positionBlock, 1) ? 1 : 0;
    }
    return result;
}

HOOK(0x004CDDD0, AimPrimaryEntryIfField0050Present, "auto");
