#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Searches the 12-byte entry table of S_00521df0 for an active entry (flag byte != 0)
// whose key float (+4) equals a2[3] and whose linked object has the position a2[0..2]
// at its +0x1C..+0x24 floats. Returns the entry index, or -1 if none matches or the table is empty.
static int __fastcall S_00521df0_FindEntryIndexByPosition(S_00521df0* self, void* /*edx*/, float* a2) {
    const uint32_t entryCount = self->field_0054;
    if (entryCount == 0) {
        return -1;
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_004c);
    uint32_t index = 0;
    do {
        if (CSF_AT(uint8_t, entry, 0) != 0 && CSF_AT(float, entry, 4) == a2[3]) {
            uint8_t* target = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 8));
            if (CSF_AT(float, target, 0x1C) == a2[0] &&
                CSF_AT(float, target, 0x20) == a2[1] &&
                CSF_AT(float, target, 0x24) == a2[2]) {
                return static_cast<int>(index);
            }
        }
        entry += 12;
        ++index;
    } while (index < entryCount);

    return -1;
}

HOOK(0x00522160, S_00521df0_FindEntryIndexByPosition, "auto");
