#include <cstdint>
#include "globals.h"
#include "hook.h"

// Pointer to the global options object (COptions*).
CSF_GLOBAL(uint8_t*, g_pOptions, 0x8BAF8C);

// Reads the byte stored at offset 0x68 of the options object and returns it.
static char __cdecl GetOptionsByte104()
{
    return static_cast<char>(g_pOptions[0x68]);
}

HOOK(0x006973E0, GetOptionsByte104, "auto");
