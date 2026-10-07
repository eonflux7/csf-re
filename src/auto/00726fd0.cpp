#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword stored at self->field_0048 + a2 * 36.
static int __fastcall C_008655e8_GetEntryAtIndex(C_008655e8* self, void* /*edx*/, int a2)
{
    uint32_t base = self->field_0048;
    return *reinterpret_cast<int*>(base + 36 * a2);
}

HOOK(0x00726FD0, C_008655e8_GetEntryAtIndex, "auto");
