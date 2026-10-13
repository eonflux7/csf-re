#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a 12-byte record header from src into the object at +0x5C..+0x67.
// Returns the 16-bit value stored at src+10 (the value left in ax).
static int16_t __fastcall NetCallbacks_CopyRecordFromSource(void* self, void* /*edx*/, const void* src) {
    CSF_AT(uint8_t, self, 0x5C) = CSF_AT(uint8_t, src, 0);
    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, src, 4);
    CSF_AT(uint16_t, self, 0x64) = CSF_AT(uint16_t, src, 8);
    int16_t result = CSF_AT(int16_t, src, 10);
    CSF_AT(int16_t, self, 0x66) = result;
    return result;
}

HOOK(0x7435C0, NetCallbacks_CopyRecordFromSource, "auto");
