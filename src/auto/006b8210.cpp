#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the attachment record at this+0x10 has kind 2 or 4 (secondary pass), else 0.
static int __fastcall C_0085ec18_GetAttachmentSecondaryPassFlag(C_0085ec18* self, void* /*edx*/) {
    uint32_t attachment = CSF_AT(uint32_t, self, 0x10);
    int result = 0;
    if (attachment != 0) {
        int attachmentKind = CSF_AT(uint8_t, reinterpret_cast<void*>(attachment), 0x0C);
        if (attachmentKind == 2 || attachmentKind == 4) {
            return 1;
        }
    }
    return result;
}

HOOK(0x6B8210, C_0085ec18_GetAttachmentSecondaryPassFlag, "auto");
