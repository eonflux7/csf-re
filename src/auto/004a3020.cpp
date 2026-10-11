#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Slot 0x80 of C_008344f0: returns an index that selects a 16-byte entry.
// Slot 0x7C of C_008344f0: performs the indexed operation with five stack args.
// Each takes (this, edx, ...) as a __fastcall-style free function.

static int __fastcall C_008344f0_DispatchIndexedCall(C_008344f0* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    // v6 = this + 16 * SlotIndex(a2)
    const uint32_t entryIndex = static_cast<uint32_t>(
        ((int (__fastcall*)(void*, void*, int))self->vtbl[0x80 / 4])(self, nullptr, a2));
    uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 16u * entryIndex;

    CSF_AT(uint32_t, entry, 0x14) = static_cast<uint32_t>(a2);

    const int resultFromSlot70 = self->vSlot70(a2);
    CSF_AT(uint32_t, entry, 0x18) = static_cast<uint32_t>(resultFromSlot70);

    int result = ((int (__fastcall*)(void*, void*, int, int, int, int, int))self->vtbl[0x7C / 4])(
        self, nullptr, a2, resultFromSlot70, a3, a4, a5);

    if (result == -1) {
        // Release the child object held at this->field_0008 (its subobject at +4).
        uint8_t* holder = reinterpret_cast<uint8_t*>(self->field_0008) + 4;
        void** holderVtbl = CSF_AT(void**, holder, 0);
        ((void (__fastcall*)(void*, void*))holderVtbl[12 / 4])(holder, nullptr);
        return -1;
    }
    return result;
}

HOOK(0x4A3020, C_008344f0_DispatchIndexedCall, "auto");
