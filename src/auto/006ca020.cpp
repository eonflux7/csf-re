#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Global dword whose address this accessor hands back.
CSF_GLOBAL(uint32_t, g_8BB420, 0x8BB420);

// Returns the address of the global object/dword at 0x008BB420.
// (Original: `mov eax, 0x8BB420 ; ret`)
static uint32_t* __fastcall C_00860a30_GetGlobal8BB420(C_00860a30* self, void* /*edx*/)
{
    (void)self;
    return &g_8BB420;
}

HOOK(0x006CA020, C_00860a30_GetGlobal8BB420, "auto");
