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

// Copies a 40-byte DDL record from src into the object's record block at +0x44..+0x68.
// Field-for-field copy in the original's order; returns the last dword copied (src+0x24).
static int __fastcall NetCogibleDDL_CopyRecordBlock(void* self, void* /*edx*/, void* src) {
    CSF_AT(uint32_t, self, 0x44) = CSF_AT(uint32_t, src, 0x00);
    CSF_AT(uint16_t, self, 0x48) = CSF_AT(uint16_t, src, 0x04);
    CSF_AT(uint16_t, self, 0x4A) = CSF_AT(uint16_t, src, 0x06);
    CSF_AT(uint32_t, self, 0x4C) = CSF_AT(uint32_t, src, 0x08);
    CSF_AT(uint32_t, self, 0x50) = CSF_AT(uint32_t, src, 0x0C);
    CSF_AT(uint32_t, self, 0x54) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, self, 0x58) = CSF_AT(uint32_t, src, 0x14);
    CSF_AT(uint32_t, self, 0x5C) = CSF_AT(uint32_t, src, 0x18);
    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, src, 0x20);
    int lastValue = CSF_AT(int, src, 0x24);
    CSF_AT(int, self, 0x68) = lastValue;
    return lastValue;
}

HOOK(0x73FA40, NetCogibleDDL_CopyRecordBlock, "auto");
