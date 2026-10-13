#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the value at this+0x4 and returns it. Declared field is absent, so CSF_AT is used.
static int __fastcall C_00840f28_SetAttachmentRecordValue(C_00840f28* self, void* /*edx*/, int value) {
    CSF_AT(int, self, 0x4) = value;
    return value;
}

HOOK(0x007283A0, C_00840f28_SetAttachmentRecordValue, "auto");
