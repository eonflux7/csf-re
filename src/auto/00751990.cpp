#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00751990 (6 bytes) - __thiscall, no stack args (ret 0)
// Sets the byte at offset +2 to 1 and returns 1.
// The original type is unknown (_BYTE*), so the field is accessed by raw offset.
static char __fastcall C_751990_SetFlagAndReturnTrue(void* self, void* /*edx*/)
{
    CSF_AT(char, self, 0x2) = 1;
    return 1;
}

HOOK(0x751990, C_751990_SetFlagAndReturnTrue, "auto");
