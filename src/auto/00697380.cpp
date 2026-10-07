#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x008BAF8C  COptions* g_pOptions
CSF_GLOBAL(void*, g_pOptions, 0x8BAF8C);

// Returns the dword field at offset 0x64 of the global options object.
static int __cdecl GetOptionsField100()
{
    return CSF_AT(int, g_pOptions, 0x64);
}

HOOK(0x00697380, GetOptionsField100, "auto");
