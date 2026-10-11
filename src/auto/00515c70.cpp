#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the 80-byte entry at position `index` inside the
// table whose base pointer is stored at field_0008 of the object.
static int __fastcall C_00840af8_GetEntryByIndex(C_00840af8* self, void* /*edx*/, int index)
{
    uint32_t tableBase = self->field_0008;
    return static_cast<int>(tableBase + 80u * static_cast<uint32_t>(index));
}

HOOK(0x515C70, C_00840af8_GetEntryByIndex, "auto");
