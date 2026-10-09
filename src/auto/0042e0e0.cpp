#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual-call signatures for the a2 interface (slots = byte offset / 4).
// a2's type is unknown, so every call goes through its vtable pointer.
using LayoutPtrSlot = void(__fastcall*)(void* self, void* edx, void* arg);
using LayoutValueSlot = void(__fastcall*)(void* self, void* edx, uint32_t arg);
using LayoutValueResultSlot = int(__fastcall*)(void* self, void* edx, uint32_t arg);

static void* GetTargetVtableSlot(void* target, int slot) {
    return CSF_AT(void**, target, 0)[slot];
}

// Pushes the HUD's stored layout values to the target object, in the original's call order.
static int __fastcall CHud_SendLayoutToTarget(CHud* self, void* /*edx*/, int a2) {
    void* target = reinterpret_cast<void*>(a2);

    // slot +0x44 (17): first stored value
    reinterpret_cast<LayoutValueSlot>(GetTargetVtableSlot(target, 0x44 / 4))(
        target, nullptr, CSF_AT(uint32_t, self, 0x2D4));

    // slot +0x4C (19): address of block at +0x2D8
    reinterpret_cast<LayoutPtrSlot>(GetTargetVtableSlot(target, 0x4C / 4))(
        target, nullptr, static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x2D8));

    // slot +0x54 (21): address of block at +0x2E8
    reinterpret_cast<LayoutPtrSlot>(GetTargetVtableSlot(target, 0x54 / 4))(
        target, nullptr, static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x2E8));

    // slot +0x60 (24): address of field_030c
    reinterpret_cast<LayoutPtrSlot>(GetTargetVtableSlot(target, 0x60 / 4))(
        target, nullptr, static_cast<void*>(&self->field_030c));

    // slot +0x68 (26): address of block at +0x2F8
    reinterpret_cast<LayoutPtrSlot>(GetTargetVtableSlot(target, 0x68 / 4))(
        target, nullptr, static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x2F8));

    // slot +0x70 (28): stored value at +0x308, returned to the caller
    return reinterpret_cast<LayoutValueResultSlot>(GetTargetVtableSlot(target, 0x70 / 4))(
        target, nullptr, CSF_AT(uint32_t, self, 0x308));
}

HOOK(0x0042E0E0, CHud_SendLayoutToTarget, "auto");
