#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies four consecutive dwords from the source block into the sub-object
// at this + 0x28 (dst[0..3] = src[0..3]) and returns the last copied dword as an int.
static int __fastcall C_00867358_CopyFourDwordsToOffset28(C_00867358* self, void* /*edx*/, const uint32_t* src)
{
    CSF_AT(uint32_t, self, 0x28) = src[0];
    CSF_AT(uint32_t, self, 0x2C) = src[1];
    CSF_AT(uint32_t, self, 0x30) = src[2];
    int result = static_cast<int>(src[3]);
    CSF_AT(uint32_t, self, 0x34) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x007389E0, C_00867358_CopyFourDwordsToOffset28, "auto");
