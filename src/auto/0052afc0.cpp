#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0052AFC0: returns the dword at offset 0x894 of C_00841370.
static int __fastcall C_00841370_GetField0894(C_00841370* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x894));
}

HOOK(0x0052AFC0, C_00841370_GetField0894, "auto");
