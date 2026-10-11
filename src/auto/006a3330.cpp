#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Zeroes three word-stride tables (286, 30 and 19 entries, 4-byte stride)
// inside the inflate state object, clears four dword counters and sets
// the word at +0x48C to 1. Returns the end cursor of the last table.
// Receives `self` in edx (ecx is unused); no stack arguments, ret 0.
static uint16_t* __fastcall Inflate_ClearLengthTables(void* /*ecx*/, void* self)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    // Table 1: 286 entries starting at +0x8C, stride 4.
    uint16_t* cursor = reinterpret_cast<uint16_t*>(base + 0x8C);
    int remaining = 286;
    do {
        *cursor = 0;
        cursor += 2;
        --remaining;
    } while (remaining != 0);

    // Table 2: 30 entries starting at +0x980, stride 4.
    cursor = reinterpret_cast<uint16_t*>(base + 0x980);
    remaining = 30;
    do {
        *cursor = 0;
        cursor += 2;
        --remaining;
    } while (remaining != 0);

    // Table 3: 19 entries starting at +0xA74, stride 4.
    uint16_t* result = reinterpret_cast<uint16_t*>(base + 0xA74);
    remaining = 19;
    do {
        *result = 0;
        result += 2;
        --remaining;
    } while (remaining != 0);

    CSF_AT(uint32_t, self, 0x16A4) = 0;
    CSF_AT(uint32_t, self, 0x16A0) = 0;
    CSF_AT(uint32_t, self, 0x16A8) = 0;
    CSF_AT(uint32_t, self, 0x1698) = 0;
    CSF_AT(uint16_t, self, 0x48C) = 1;

    return result;
}

HOOK(0x6A3330, Inflate_ClearLengthTables, "auto");
