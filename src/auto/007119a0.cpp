#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the object stored in slot [slotIndex] of this (vtable slot 0 with arg 1),
// clears the slot, then clears the per-object flags for the slot group.
static int __fastcall C_00863d80_ReleaseSlotAndReset(C_00863d80* self, void* /*edx*/, int slotIndex) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint32_t* slotPtr = reinterpret_cast<uint32_t*>(base + 0x08 + slotIndex * 4);

    int result = 0;
    void* slotObject = reinterpret_cast<void*>(*slotPtr);
    if (slotObject) {
        using ReleaseFn = int(__fastcall*)(void*, void*, int);
        ReleaseFn release = reinterpret_cast<ReleaseFn>(CSF_AT(void**, slotObject, 0)[0]);
        result = release(slotObject, nullptr, 1);
        *slotPtr = 0;
    }

    self->field_01b3 = 0;
    if (slotIndex) {
        self->field_00e0 = 0;
    } else {
        self->field_0018 = 0;
    }
    return result;
}

HOOK(0x007119A0, C_00863d80_ReleaseSlotAndReset, "auto");
