#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword field at offset 0x398 of the object.
// Original: mov eax, [ecx+0x398]; ret
static int __fastcall C_004950e0_GetField398(void* self, void* /*edx*/)
{
    return CSF_AT(int, self, 0x398);
}

HOOK(0x4950E0, C_004950e0_GetField398, "auto");
