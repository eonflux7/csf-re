#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Each entry record is 28 (0x1C) bytes; the signed dword at +8 gates the entry.
// Returns true when that dword is >= 0 (signed compare, matching setge).
static bool __fastcall IsEntryFrameBlocked(C_008342e8* self, void* /*edx*/, int entryIndex) {
    uint8_t* entryTable = reinterpret_cast<uint8_t*>(self->field_0198);
    int32_t entryState = CSF_AT(int32_t, entryTable, 28 * entryIndex + 8);
    return entryState >= 0;
}

HOOK(0x0049ED50, IsEntryFrameBlocked, "auto");
