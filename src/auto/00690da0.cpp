#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global data block whose address this function returns.
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x8BAD98);

// Returns the address of the static data block at 0x8BAD98.
static void* __cdecl GetData_8BAD98()
{
    return &g_unk_8BAD98;
}

HOOK(0x690DA0, GetData_8BAD98, "auto");
