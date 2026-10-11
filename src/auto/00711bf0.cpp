#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Dispatches vtable slot 11 (+0x2c) on the child object stored at this+8+index*4.
// Returns the child's slot-11 result, or 0 when the child slot is empty.
static int __fastcall C_00863d80_InvokeIndexedChildSlot11(C_00863d80* self, void* /*edx*/, int childIndex) {
    uint32_t childAddress = CSF_AT(uint32_t, self, 0x8 + childIndex * 4);
    void* child = reinterpret_cast<void*>(childAddress);
    if (child) {
        auto slot11 = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, child, 0)[11]);
        return slot11(child, nullptr);
    }
    return 0;
}

HOOK(0x00711BF0, C_00863d80_InvokeIndexedChildSlot11, "auto");
