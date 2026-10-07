#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Pointer to the global options object (COptions*).
CSF_GLOBAL(uint8_t*, g_pOptions, 0x8BAF8C);

// Reads the byte-sized flag stored at offset 1 of the options object.
static char __cdecl GetOptionsByte1()
{
    return static_cast<char>(CSF_AT(uint8_t, g_pOptions, 0x1));
}

HOOK(0x697320, GetOptionsByte1, "auto");
