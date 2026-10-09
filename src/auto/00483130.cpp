#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the mask id stored in the object at offset 0x90 (dword index 36).
static uint32_t __fastcall CharActionFsm_GetMaskId(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x90);
}

HOOK(0x00483130, CharActionFsm_GetMaskId, "auto");
