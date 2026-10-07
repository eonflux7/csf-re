#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns byte at offset 0x130 (inside base pad_012c).
static uint8_t __fastcall CGuiControl_GetField0130(CGuiControl* self, void* /*edx*/)
{
    return CSF_AT(uint8_t, self, 0x130);
}

HOOK(0x00437070, CGuiControl_GetField0130, "auto");
