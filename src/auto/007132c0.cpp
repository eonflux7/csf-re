#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a pointer to the embedded sub-object/array at offset 0xB30 within
// the C_00863ed8 object. (0xB30 lies inside the pad_0230[0xa00] block.)
static char* __fastcall C_00863ed8_GetEmbedded0B30(C_00863ed8* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0xb30;
}

HOOK(0x007132C0, C_00863ed8_GetEmbedded0B30, "auto");
