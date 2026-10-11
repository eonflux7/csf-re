#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_00833908_IsMissingRequiredFlags(
    C_00833908* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 0x170) & 6) != 6;
}

HOOK(0x0048EDC0, C_00833908_IsMissingRequiredFlags, "auto");
