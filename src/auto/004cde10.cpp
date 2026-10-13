#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Checks the target handle against the object's source handle (this+0x40 sub-object).
CSF_FUNC(bool, __fastcall, S_0048c6c0_CheckTarget, 0x0048C720, (S_0048c6c0* self, void* /*edx*/, int sourceHandle, int targetHandle));

// Sends the order record to the controller stored at (field_0014 -> +0x44).
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* /*edx*/, int taskIndex, int orderAddress));

// Builds a 0x24-byte order record on the stack and sends it to the task controller.
// Ret 4: __thiscall with one stack argument (orderData).
static int __fastcall S_0048c6c0_SendTargetOrder(S_0048c6c0* self, void* /*edx*/, int orderData)
{
    auto* orderPtr = reinterpret_cast<uint8_t*>(orderData);

    bool checkResult = S_0048c6c0_CheckTarget(
        reinterpret_cast<S_0048c6c0*>(reinterpret_cast<uint8_t*>(self) + 0x40), nullptr,
        static_cast<int>(self->field_0014 + 0x1C),
        orderData);

    alignas(8) uint8_t orderBuffer[0x24];

    CSF_AT(uint8_t, orderBuffer, 0x20) = 1;
    CSF_AT(uint8_t, orderBuffer, 0x22) = 1;
    CSF_AT(int, orderBuffer, 0x0C) = CSF_AT(int, orderPtr, 0x04);
    CSF_AT(int, orderBuffer, 0x00) = 0;
    CSF_AT(void*, orderBuffer, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, orderBuffer, 0x04) = static_cast<int>(checkResult) + 1;
    CSF_AT(int, orderBuffer, 0x10) = CSF_AT(int, orderPtr, 0x08);
    CSF_AT(uint32_t, orderBuffer, 0x14) = 0;
    CSF_AT(uint32_t, orderBuffer, 0x18) = 0;
    CSF_AT(uint32_t, orderBuffer, 0x1C) = 0;
    CSF_AT(uint8_t, orderBuffer, 0x21) = 0;

    uint8_t* controllerBase = reinterpret_cast<uint8_t*>(self->field_0014);
    C_00833708* controller = CSF_AT(C_00833708*, controllerBase, 0x44);

    uint32_t* sendResult = C_00833708_SendOrder(controller, nullptr, 0, reinterpret_cast<int>(orderBuffer));
    return reinterpret_cast<int>(sendResult);
}

HOOK(0x004CDE10, S_0048c6c0_SendTargetOrder, "auto");
