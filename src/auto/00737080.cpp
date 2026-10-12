#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Replaces the object's current attached item (+0x1c) with a2. The old item is
// released through the vtable of the object at +0x0c (slot 0x8c). When a2 is
// non-null, its two payload dwords (+0x0c, +0x10) are copied into this object's
// +0x7c/+0x80 and the object's vtable slot 0x38 is called with a pointer to the
// copied payload. Returns a2.
static int __fastcall C_00867260_SetBoundPayload(C_00867260* self, void* /*edx*/, int a2)
{
    uint32_t previousItem = CSF_AT(uint32_t, self, 0x1c);
    CSF_AT(uint8_t, self, 0x20) = 1;

    if (previousItem) {
        void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0c));
        // Slot 0x8c / 4 = 35: void method taking the previous item.
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, owner, 0)[35])(
            owner, nullptr, previousItem);
    }

    int result = a2;
    CSF_AT(int, self, 0x1c) = a2;

    if (a2) {
        CSF_AT(uint32_t, self, 0x7c) = CSF_AT(uint32_t, a2, 0x0c);
        CSF_AT(uint32_t, self, 0x80) = CSF_AT(uint32_t, a2, 0x10);
        // Slot 0x38 / 4 = 14: int method taking a pointer to the copied payload.
        return ((int(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[14])(
            self, nullptr, &CSF_AT(uint32_t, self, 0x7c));
    }
    return result;
}

HOOK(0x737080, C_00867260_SetBoundPayload, "auto");
