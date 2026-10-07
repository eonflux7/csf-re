#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the 32-bit value stored at offset 0x338 of the object.
// Original body: mov eax, [ecx+0x338] ; ret
static int __fastcall GetField338(void* self, void* /*edx*/)
{
    return CSF_AT(int, self, 0x338);
}

HOOK(0x6A7170, GetField338, "auto");
