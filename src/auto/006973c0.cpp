#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// g_pOptions (0x8BAF8C) points to the game's COptions singleton.
// Byte 104 (0x68) is a plain option/flag byte; its meaning is not known.
CSF_GLOBAL(uint8_t*, g_pOptions, 0x8BAF8C);

// Stores the argument into g_pOptions->field_0068 and returns that same value.
static char __cdecl StoreOptionsByte104(char value)
{
    CSF_AT(char, g_pOptions, 0x68) = value;
    return value;
}

HOOK(0x6973C0, StoreOptionsByte104, "auto");
