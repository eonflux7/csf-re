#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the sub-object embedded at offset 0xB0 of this instance.
static C_00864288* __fastcall C_00864288_GetField0B0(C_00864288* self, void* /*edx*/)
{
    return reinterpret_cast<C_00864288*>(reinterpret_cast<char*>(self) + 0xb0);
}

HOOK(0x718040, C_00864288_GetField0B0, "auto");
