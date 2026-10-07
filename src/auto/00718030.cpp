#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the sub-object embedded at offset 0xD8 of this instance.
static char* __fastcall C_00864288_GetSubObject0D8(C_00864288* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0xd8;
}

HOOK(0x00718030, C_00864288_GetSubObject0D8, "auto");
