#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00833708_SendOrder, 0x0048A4D0, (C_00833708* self, void* , int taskIndex, int orderAddress));

// Resets the task by building a 0x24-byte order block on the stack and sending it
// to the order port reached through this->field_0014->field_0044.
static uint32_t* __fastcall C_0083c280_ResetTask(C_0083c280* self, void* /*edx*/)
{
    uint8_t* ownerObject = reinterpret_cast<uint8_t*>(self->field_0014);
    C_00833708* orderPort = reinterpret_cast<C_00833708*>(CSF_AT(uint32_t, ownerObject, 0x44));

    // Stack order block (0x24 bytes, same layout as the original frame).
    alignas(8) uint8_t orderBytes[0x24];
    std::memset(orderBytes, 0, sizeof(orderBytes));

    CSF_AT(uint32_t, orderBytes, 0x00) = 2;                                            // [ebp-0x24]
    CSF_AT(uint32_t, orderBytes, 0x04) = 0;                                            // [ebp-0x20]
    CSF_AT(uint32_t, orderBytes, 0x08) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448)); // [ebp-0x1C]
    CSF_AT(uint32_t, orderBytes, 0x0C) = 0xFFFFFFFFu;                                  // [ebp-0x18]
    CSF_AT(uint32_t, orderBytes, 0x10) = 0xFFFFFFFFu;                                  // [ebp-0x14]
    CSF_AT(uint32_t, orderBytes, 0x14) = 0;                                            // [ebp-0x10]
    CSF_AT(uint32_t, orderBytes, 0x18) = 0;                                            // [ebp-0x0C]
    CSF_AT(uint32_t, orderBytes, 0x1C) = 0;                                            // [ebp-0x08]
    CSF_AT(uint8_t, orderBytes, 0x20) = 1;                                             // [ebp-0x04]
    CSF_AT(uint8_t, orderBytes, 0x21) = 0;                                             // [ebp-0x03]
    CSF_AT(uint8_t, orderBytes, 0x22) = 1;                                             // [ebp-0x02]

    return C_00833708_SendOrder(orderPort, nullptr, 0, reinterpret_cast<int>(orderBytes));
}

HOOK(0x004D5840, C_0083c280_ResetTask, "auto");
