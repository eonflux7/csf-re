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

// Returns bit 6 of the flag byte at +0x452 (1106): 1 when the player is excluded.
static int __fastcall CMenuMpStats_GetPlayerExclusionFlag(CMenuMpStats* self, void* /*edx*/)
{
    uint8_t flagByte = CSF_AT(uint8_t, self, 0x452);
    return (flagByte >> 6) & 1;
}

HOOK(0x007411A0, CMenuMpStats_GetPlayerExclusionFlag, "auto");
