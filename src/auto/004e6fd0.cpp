#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns non-zero when the result slot at a2 was written (state != -1).
static int __fastcall C_0083eb70_TryClaimResultSlot(C_0083eb70* self, void* /*edx*/, int a2) {
    uint8_t* result = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(int, result, 0x04) = -1;

    // Nested object at self->field_0008; its state byte-dword lives at +0x1D8.
    uint8_t* linkedObject = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t linkedState = CSF_AT(uint32_t, linkedObject, 0x1D8);

    if (linkedState != 3 && linkedState != 2 &&
        self->vSlot9C(static_cast<int>(a2 + 0x14), 3)) {
        CSF_AT(int, result, 0x04) = 1;
        CSF_AT(int, result, 0x08) = 3;
    }

    return CSF_AT(int, result, 0x04) != -1 ? 1 : 0;
}

HOOK(0x004E6FD0, C_0083eb70_TryClaimResultSlot, "auto");
