#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x14 / 4 = 5 on the entry objects. The entry's concrete type is unknown,
// so the call goes through CSF_AT on its vtable. The original passes no arguments.
static char __fastcall RpSkin_UpdateEntriesIfEnabled(S_006c5cd0* self, void* /*edx*/) {
    // Bit 0x100 of field_0008 gates the work (test ah, 1).
    if ((self->field_0008 & 0x100) == 0 || self->field_0058 == 0) {
        return 1;
    }

    uint32_t index = 0;
    do {
        // Re-read the entry array pointer on every iteration, as the original does.
        void* entry = reinterpret_cast<void**>(self->field_0050)[index];
        if (entry != nullptr) {
            // Slot 5 (byte offset 0x14) of the entry's vtable, __thiscall with no arguments.
            using EntryFn = void(__fastcall*)(void* self, void* edx);
            EntryFn update = reinterpret_cast<EntryFn>(CSF_AT(void**, entry, 0)[5]);
            update(entry, nullptr);
        }
        ++index;
        // Re-read the count after each call, since callees may change the object.
    } while (index < self->field_0058);

    return 1;
}

HOOK(0x006C5EF0, RpSkin_UpdateEntriesIfEnabled, "auto");
