#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the dword at offset 0x12c of the object.
static uint32_t __fastcall C_00864288_GetField012C(C_00864288* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x12c);
}

HOOK(0x0044F630, C_00864288_GetField012C, "auto");
