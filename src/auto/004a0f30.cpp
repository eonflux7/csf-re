#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of path point record `index` in the 12-byte-stride
// record table whose base pointer is stored at +0x04 of this object.
static int __fastcall C_0085e7b8_GetPathPointRecord(C_0085e7b8* self, void* /*edx*/, int index) {
    return static_cast<int>(self->field_0004 + static_cast<uint32_t>(index) * 12u);
}

HOOK(0x004A0F30, C_0085e7b8_GetPathPointRecord, "auto");
