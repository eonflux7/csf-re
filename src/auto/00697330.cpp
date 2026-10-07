#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Global options object pointer (COptions*) used by the game configuration module.
CSF_GLOBAL(void*, g_pOptions, 0x8BAF8C);

// Stores the given byte into the options structure at offset 0x40 and returns it.
static char __cdecl SetOptionsByteAt40(char value)
{
    CSF_AT(uint8_t, g_pOptions, 0x40) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x697330, SetOptionsByteAt40, "auto");
