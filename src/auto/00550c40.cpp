#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Clears the selection/state fields of a C_008310b8 (MP spawn point selection).
static int __fastcall C_008310b8_InitializeSelection(C_008310b8* self, void* /*edx*/) {
    // Low four bytes of the vtable pointer slot (bytes 0..3), zeroed one at a time.
    CSF_AT(uint8_t, self, 0x0) = 0;
    CSF_AT(uint8_t, self, 0x1) = 0;
    CSF_AT(uint8_t, self, 0x2) = 0;
    CSF_AT(uint8_t, self, 0x3) = 0;

    // Declared field_0004 (bytes 4..7) cleared as one dword.
    self->field_0004 = 0;

    CSF_AT(uint8_t, self, 0x87) = 0;
    CSF_AT(uint8_t, self, 0x107) = 0;

    CSF_AT(uint32_t, self, 0x188) = 0;
    CSF_AT(uint32_t, self, 0x18C) = 0;
    CSF_AT(uint32_t, self, 0x190) = 0;

    CSF_AT(uint16_t, self, 0x194) = 0;
    CSF_AT(uint16_t, self, 0x196) = 0;
    CSF_AT(uint16_t, self, 0x198) = 0;
    CSF_AT(uint16_t, self, 0x19A) = 0;
    CSF_AT(uint16_t, self, 0x19C) = 0;

    return 0;
}

HOOK(0x550C40, C_008310b8_InitializeSelection, "auto");
