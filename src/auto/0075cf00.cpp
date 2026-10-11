#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of record `recordIndex` in the 72-byte-stride record table
// whose base pointer is stored at +0x08 of this object.
static int __fastcall C_0083e0d8_GetRecordAddress(C_0083e0d8* self, void* /*edx*/, int recordIndex) {
    return static_cast<int>(self->field_0008 + static_cast<uint32_t>(recordIndex) * 72u);
}

HOOK(0x0075CF00, C_0083e0d8_GetRecordAddress, "auto");
