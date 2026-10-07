#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets two dword members (offsets 0x4C and 0x50) to -1 and returns -1.
// Matches: mov dword ptr [ecx+0x4c], eax / mov dword ptr [ecx+0x50], eax / ret
static int __fastcall C_Unknown_ResetTwoDwordsToMinusOne(void* self, void* /*edx*/)
{
    CSF_AT(int32_t, self, 0x4C) = -1;
    CSF_AT(int32_t, self, 0x50) = -1;
    return -1;
}

HOOK(0x004CFAA0, C_Unknown_ResetTwoDwordsToMinusOne, "auto");
