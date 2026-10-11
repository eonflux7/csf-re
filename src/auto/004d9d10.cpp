#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sends a 0x24-byte message to the owner object reached through self->+8->+0x14->+0x44.
// The message carries the three dwords at ((self->+0xC) + 0x3C) and uses vtable 0x82D448.
static int __fastcall AiAutonomo_NotifyOwnerWithPositionMessage(void* self, void* /*edx*/)
{
    uint8_t* hostRef = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* hostInner = CSF_AT(uint8_t*, hostRef, 0x14);
    void* ownerObject = CSF_AT(void*, hostInner, 0x44);

    uint8_t* positionBase = CSF_AT(uint8_t*, self, 0x0C) + 0x3C;
    int positionX = CSF_AT(int, positionBase, 0x00);
    int positionY = CSF_AT(int, positionBase, 0x04);
    int positionZ = CSF_AT(int, positionBase, 0x08);

    alignas(8) uint8_t messageBytes[0x24];
    CSF_AT(int, messageBytes, 0x00) = 0;
    CSF_AT(int, messageBytes, 0x04) = 3;
    CSF_AT(void*, messageBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, messageBytes, 0x0C) = -1;
    CSF_AT(int, messageBytes, 0x10) = -1;
    CSF_AT(int, messageBytes, 0x14) = positionX;
    CSF_AT(int, messageBytes, 0x18) = positionY;
    CSF_AT(int, messageBytes, 0x1C) = positionZ;
    CSF_AT(uint8_t, messageBytes, 0x20) = 0;
    CSF_AT(uint8_t, messageBytes, 0x21) = 0;
    CSF_AT(uint8_t, messageBytes, 0x22) = 0;

    using OwnerNotifyFn = int(__fastcall*)(void* self, void* edx, void* message);
    OwnerNotifyFn notifyOwner = (OwnerNotifyFn)CSF_AT(void**, ownerObject, 0)[22];
    return notifyOwner(ownerObject, nullptr, messageBytes);
}

HOOK(0x004D9D10, AiAutonomo_NotifyOwnerWithPositionMessage, "auto");
