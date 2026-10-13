#include <cstdint>
#include "globals.h"
#include "hook.h"

// Fills a table of four 20-byte records. Each record is:
//   +0x00 float 75.0f
//   +0x04 float 25.0f
//   +0x08 int   10
//   +0x0C int   80
//   +0x10 float 4000.0f
// Returns the same object pointer (this).
static void* __fastcall InitDefaultSlotTable(void* self, void* /*edx*/) {
    // Record 0 (base +0x00)
    CSF_AT(float, self, 0x00) = 75.0f;
    CSF_AT(float, self, 0x04) = 25.0f;
    CSF_AT(int, self, 0x08) = 10;
    CSF_AT(int, self, 0x0C) = 80;
    CSF_AT(float, self, 0x10) = 4000.0f;

    // Record 1 (base +0x14)
    CSF_AT(float, self, 0x14) = 75.0f;
    CSF_AT(float, self, 0x18) = 25.0f;
    CSF_AT(int, self, 0x1C) = 10;
    CSF_AT(int, self, 0x20) = 80;
    CSF_AT(float, self, 0x24) = 4000.0f;

    // Record 2 (base +0x28)
    CSF_AT(float, self, 0x28) = 75.0f;
    CSF_AT(float, self, 0x2C) = 25.0f;
    CSF_AT(int, self, 0x30) = 10;
    CSF_AT(int, self, 0x34) = 80;
    CSF_AT(float, self, 0x38) = 4000.0f;

    // Record 3 (base +0x3C)
    CSF_AT(float, self, 0x3C) = 75.0f;
    CSF_AT(float, self, 0x40) = 25.0f;
    CSF_AT(int, self, 0x44) = 10;
    CSF_AT(int, self, 0x48) = 80;
    CSF_AT(float, self, 0x4C) = 4000.0f;

    return self;
}

HOOK(0x00782590, InitDefaultSlotTable, "auto");
