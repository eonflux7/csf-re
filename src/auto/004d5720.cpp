#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Order-sending entry point of the task manager (0x0048A4D0).
CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0,
         (C_00833708* self, void* /*edx*/, int taskIndex, int orderAddress));

// Builds a 0x24-byte order record on the stack from the 3-dword item payload
// and sends it to the dispatcher's order sink (dispatcher+0x44) as task 0.
static uint32_t* __fastcall C_0083c280_DispatchItemAction(C_0083c280* self, void* /*edx*/,
                                                          uint32_t* itemPayload, char actionFlag) {
    // Dispatcher object referenced by this->field_0014.
    void* dispatcher = reinterpret_cast<void*>(self->field_0014);

    alignas(8) uint8_t orderBytes[0x24];
    CSF_AT(uint32_t, orderBytes, 0x00) = 0;
    CSF_AT(uint32_t, orderBytes, 0x04) = 3;
    CSF_AT(void**, orderBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, orderBytes, 0x0C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, orderBytes, 0x10) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, orderBytes, 0x14) = itemPayload[0];
    CSF_AT(uint32_t, orderBytes, 0x18) = itemPayload[1];
    CSF_AT(uint32_t, orderBytes, 0x1C) = itemPayload[2];
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;
    CSF_AT(char, orderBytes, 0x21) = actionFlag;
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;

    // Order sink lives at dispatcher+0x44.
    uint32_t orderSinkAddress = CSF_AT(uint32_t, dispatcher, 0x44);
    C_00833708* orderSink = reinterpret_cast<C_00833708*>(orderSinkAddress);

    return C_00833708_SendOrder(orderSink, nullptr, 0,
                                static_cast<int>(reinterpret_cast<uintptr_t>(orderBytes)));
}

HOOK(0x004D5720, C_0083c280_DispatchItemAction, "auto");
