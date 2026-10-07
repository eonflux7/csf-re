#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns bit 0 of the dword at offset +4 of the object.
static int __fastcall GetFlagBit0At4(void* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 4) & 1u);
}

HOOK(0x006B8120, GetFlagBit0At4, "auto");
