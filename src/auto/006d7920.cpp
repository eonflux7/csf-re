#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void* __fastcall C_008611d4_GetGroup(C_008611d4* self, void* /*edx*/)
{
    return CSF_AT(void*, self, 0x100);
}

HOOK(0x006D7920, C_008611d4_GetGroup, "auto");
