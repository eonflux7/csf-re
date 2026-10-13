#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the render key of the attached object at +0x10, or 32 when nothing is attached.
static int __fastcall C_0085ec18_GetAttachmentRenderKey(C_0085ec18* self, void* /*edx*/)
{
    uint32_t attachedObject = CSF_AT(uint32_t, self, 0x10);
    if (attachedObject != 0)
        return CSF_AT(int, reinterpret_cast<uint8_t*>(attachedObject), 0x10);
    return 32;
}

HOOK(0x6B8250, C_0085ec18_GetAttachmentRenderKey, "auto");
