#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the two child objects held at +0x08 and +0x0C (calling each child's
// vtable slot 0 with argument 1, then nulling the pointer), then clears the
// flag bytes at +0x18, +0xE0 and +0x1B3.
static int __fastcall C_00863d80_ReleaseChildrenAndClearFlags(C_00863d80* self, void* /*edx*/)
{
    int result = 0;
    uint32_t* childSlot = &self->field_0008;

    for (int remaining = 2; remaining != 0; --remaining) {
        if (*childSlot) {
            void* child = reinterpret_cast<void*>(*childSlot);
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
            *childSlot = 0;
        }
        ++childSlot;
    }

    self->field_0018 = 0;
    self->field_00e0 = 0;
    self->field_01b3 = 0;
    return result;
}

HOOK(0x711960, C_00863d80_ReleaseChildrenAndClearFlags, "auto");
