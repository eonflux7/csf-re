#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the attachment id stored in the low 16 bits of the object's first dword
// (the vtable slot position), or -1 when those bits are 0xFFFF.
static int __fastcall GfxEnvironment_GetAttachmentId(C_0085ec18* self, void* /*edx*/)
{
    int attachmentId = static_cast<int>(CSF_AT(uint32_t, self, 0) & 0xFFFFu);
    if (attachmentId == 0xFFFF)
        return -1;
    return attachmentId;
}

HOOK(0x6B8080, GfxEnvironment_GetAttachmentId, "auto");
