#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns bit 10 (0x400) of the flags dword at this+4: 1 when the attachment is a COM array.
// DName is not declared in auto_classes.h, so the object is addressed as a raw pointer.
static int __fastcall IsAttachmentComArray(void* self, void* /*edx*/)
{
    uint32_t flagsWord = CSF_AT(uint32_t, self, 0x4);
    return (flagsWord >> 10) & 1;
}

HOOK(0x6B8130, IsAttachmentComArray, "auto");
