#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a table of 30 slots (48 bytes each, starting at this+0x1C), then clears the leading byte.
static uint8_t* __fastcall ResetSessionSlotTable(S_0073d4d0* self, void* /*edx*/) {
    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 0x1C;
    for (int remaining = 30; remaining != 0; --remaining) {
        CSF_AT(uint32_t, cursor, 4) = 7;
        CSF_AT(uint32_t, cursor, 0) = 0;
        CSF_AT(uint16_t, cursor - 0x10, 0) = 0;
        cursor += 0x30;
    }
    self->field_0000 = 0;
    return &self->field_0000;
}

HOOK_KEEP_ECX(0x73D4D0, ResetSessionSlotTable, 0, "net_session");
