#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the attachment record at this+0x10 has kind 1 or 3 (primary pass), else 0.
static int __fastcall C_0085ec18_GetAttachmentPrimaryPassFlag(C_0085ec18* self, void* /*edx*/) {
    uint32_t attachment = CSF_AT(uint32_t, self, 0x10);
    int result = 0;
    if (attachment != 0) {
        int attachmentKind = CSF_AT(uint8_t, reinterpret_cast<void*>(attachment), 0x0C);
        if (attachmentKind == 1 || attachmentKind == 3) {
            return 1;
        }
    }
    return result;
}

HOOK(0x6B81F0, C_0085ec18_GetAttachmentPrimaryPassFlag, "auto");
