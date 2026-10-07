#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the embedded sub-object located at offset 0xa8 of this.
// 007349e0: lea eax, [ecx+0xa8]; ret
static char* __fastcall C_008670f0_GetEmbeddedA8(C_008670f0* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0xa8;
}

HOOK(0x7349E0, C_008670f0_GetEmbeddedA8, "auto");
