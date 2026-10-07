#include <cstdint>
#include "globals.h"
#include "hook.h"

// g_pOptions at 0x008BAF8C points to the game options structure (COptions*).
// This accessor returns the byte stored at offset +2 of that structure.
CSF_GLOBAL(uint8_t*, g_pOptions, 0x8BAF8C);

static char __cdecl GetOptionsByteAt2()
{
    return static_cast<char>(g_pOptions[2]);
}

HOOK(0x006972F0, GetOptionsByteAt2, "auto");
