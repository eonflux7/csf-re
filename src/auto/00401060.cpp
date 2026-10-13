#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 3-dword trace (origin/direction triple) from `source` into `self`,
// then stores `extra` in the fourth dword. Returns `self`.
static void* __fastcall CharacterModel_InitializeTrace(void* self, void* /*edx*/, int a2, const uint32_t* a3)
{
    CSF_AT(uint32_t, self, 0x00) = a3[0];
    CSF_AT(uint32_t, self, 0x04) = a3[1];
    CSF_AT(uint32_t, self, 0x08) = a3[2];
    CSF_AT(int, self, 0x0C) = a2;
    return self;
}

HOOK(0x00401060, CharacterModel_InitializeTrace, "auto");
