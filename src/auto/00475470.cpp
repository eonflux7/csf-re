#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global pointer singleton at 0x008AFC90.
CSF_GLOBAL(void*, g_pGlobalObject_8AFC90, 0x8AFC90);

// Reads a 32-bit field at offset 0x184 from the object referenced by the
// global pointer at 0x008AFC90 and returns it.
static int __cdecl GetGlobalObjectField184()
{
    return CSF_AT(uint32_t, g_pGlobalObject_8AFC90, 0x184);
}

HOOK(0x475470, GetGlobalObjectField184, "auto");
