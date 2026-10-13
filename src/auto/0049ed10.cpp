#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of control entry `index` in the table whose base pointer is
// stored at this+0x198. Each entry is 28 (0x1C) bytes.
static int __fastcall C_008342e8_GetControlEntryByIndex(C_008342e8* self, void* /*edx*/, int index)
{
    int tableBase = static_cast<int>(self->field_0198);
    return tableBase + index * 28;
}

HOOK(0x0049ED10, C_008342e8_GetControlEntryByIndex, "auto");
