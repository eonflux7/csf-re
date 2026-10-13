#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at offset 0x1C of entry a2 in the table whose base pointer is stored at self+0x08.
// Entries are 36 bytes each.
static int __fastcall AccessorState_ResolveSlot(void* self, void* /*edx*/, int a2)
{
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x08);
    uint8_t* entry = reinterpret_cast<uint8_t*>(tableBase) + 36 * a2;
    return CSF_AT(int, entry, 0x1C);
}

HOOK(0x006C8EE0, AccessorState_ResolveSlot, "auto");
