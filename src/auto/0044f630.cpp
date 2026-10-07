#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the dword at offset 0x12c of the object.
static uint32_t __fastcall CGuiControl_GetField012C(CGuiControl* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x12c);
}

HOOK(0x0044F630, CGuiControl_GetField012C, "auto");
