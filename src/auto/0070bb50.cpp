#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies three dwords from the block at (*a2 + 0x30) into this+0x8..+0x10 and returns the middle dword.
static int __fastcall C_00863760_CopyStateFromSource(C_00863760* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* src = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2[0]) + 0x30);
    self->field_0008 = src[0];
    uint32_t middle = src[1];
    CSF_AT(uint32_t, self, 0x0C) = middle;
    CSF_AT(uint32_t, self, 0x10) = src[2];
    return static_cast<int>(middle);
}

HOOK(0x0070BB50, C_00863760_CopyStateFromSource, "auto");
