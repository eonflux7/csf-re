#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the 4-byte value at offset 0x180 of the object (a field inside the
// inherited padding area, not part of the declared CGuiListBoxBase fields).
static int __fastcall CGuiListBoxBase_GetField180(CGuiListBoxBase* self, void* /*edx*/)
{
    return CSF_AT(int, self, 0x180);
}

HOOK(0x00716310, CGuiListBoxBase_GetField180, "auto");
