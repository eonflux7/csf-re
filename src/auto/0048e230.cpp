#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword stored at offset 0x16C of C_00833908 (field_016c).
static int __fastcall C_00833908_GetField016c(C_00833908* self, void* /*edx*/)
{
    return static_cast<int>(self->field_016c);
}

HOOK(0x48E230, C_00833908_GetField016c, "auto");
