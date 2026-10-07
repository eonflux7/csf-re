#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Pointer to the global options object (COptions*), stored at 0x8BAF8C.
CSF_GLOBAL(void*, g_pOptions, 0x8BAF8C);

// Returns the byte flag held at offset 0x40 of the global options object.
static char __cdecl GetOptionsByte40()
{
    return CSF_AT(char, g_pOptions, 0x40);
}

HOOK(0x697350, GetOptionsByte40, "auto");
