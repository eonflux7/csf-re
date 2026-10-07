#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the 32-bit value stored at offset 0x168 of the object
// (inherited portion of the class, not part of CGuiButton's own declared fields).
static uint32_t __fastcall CGuiButton_GetField0168(CGuiButton* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x168);
}

HOOK(0x00724EF0, CGuiButton_GetField0168, "auto");
