#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the 64-entry key/device state table that starts at this+4.
// Each entry is 0x24 bytes: a dword at +0 (set to 6 last), dwords at +4..+0x1C
// zeroed, and four bytes at +0x20..+0x23 zeroed. The leading dword of the object
// (this+0) is cleared at the end. Returns the object pointer.
static uint32_t* __fastcall InputDinput_ResetKeyStateTable(uint8_t* self, void* /*edx*/) {
    uint32_t* result = reinterpret_cast<uint32_t*>(self);
    constexpr int kEntryCount = 64;
    constexpr int kEntryStride = 0x24;
    constexpr int kTableOffset = 4;

    for (int i = 0; i < kEntryCount; ++i) {
        uint8_t* entry = self + kTableOffset + i * kEntryStride;
        CSF_AT(uint32_t, entry, 0x04) = 0;
        CSF_AT(uint32_t, entry, 0x08) = 0;
        CSF_AT(uint32_t, entry, 0x0C) = 0;
        CSF_AT(uint32_t, entry, 0x10) = 0;
        CSF_AT(uint32_t, entry, 0x14) = 0;
        CSF_AT(uint32_t, entry, 0x18) = 0;
        CSF_AT(uint32_t, entry, 0x1C) = 0;
        CSF_AT(uint8_t, entry, 0x20) = 0;
        CSF_AT(uint8_t, entry, 0x21) = 0;
        CSF_AT(uint8_t, entry, 0x22) = 0;
        CSF_AT(uint8_t, entry, 0x23) = 0;
        CSF_AT(uint32_t, entry, 0x00) = 6;
    }

    CSF_AT(uint32_t, self, 0x00) = 0;
    return result;
}

HOOK(0x712EB0, InputDinput_ResetKeyStateTable, "auto");
