#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, IsField90Null, 0x0040C910, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiSearchOrders_AimControllerAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));

// Returns the result of IsField90Null on the object held at this->field_0014->field_004C.
// If that check is true, runs AiSearchOrders_AimControllerAtPosition on that same object,
// passing the float block at this+0x80 and flag 1.
static int __fastcall AimControllerIfField90Present(void* self, void* /*edx*/) {
    uint32_t linkedPtr = CSF_AT(uint32_t, self, 0x14);
    void* linked = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedPtr));
    uint32_t fieldPtr = CSF_AT(uint32_t, linked, 0x4C);
    void* fieldObj = reinterpret_cast<void*>(static_cast<uintptr_t>(fieldPtr));

    int result = IsField90Null(fieldObj, nullptr);
    if ((uint8_t)result) {
        float* aimBlock = reinterpret_cast<float*>(static_cast<uint8_t*>(self) + 0x80);
        return AiSearchOrders_AimControllerAtPosition(fieldObj, nullptr, aimBlock, 1) ? 1 : 0;
    }
    return result;
}

HOOK(0x004A8DE0, AimControllerIfField90Present, "auto");
