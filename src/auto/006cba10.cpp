#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the int at +0x3C of the entry at `index` in the table at field_00a0,
// or 0 when the enabled flag (field_0096) is clear.
static int __fastcall C_00860bb8_GetEntryDwordAtIndex(C_00860bb8* self, void* /*edx*/, int index) {
    if (self->field_0096) {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_00a0)[index];
        uint8_t* entryObject = reinterpret_cast<uint8_t*>(entry);
        return CSF_AT(int, entryObject, 0x3C);
    }
    return 0;
}

HOOK(0x006CBA10, C_00860bb8_GetEntryDwordAtIndex, "auto");
