#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies four consecutive dwords from the source block into this + 0x24
// (dst[0..3] = src[0..3]) and returns the last copied dword as a 32-bit int.
static int __fastcall CopyFourDwordsIntoLightAt24(C_00862e88* self, void* /*edx*/, const uint32_t* src)
{
    CSF_AT(uint32_t, self, 0x24) = src[0];
    CSF_AT(uint32_t, self, 0x28) = src[1];
    CSF_AT(uint32_t, self, 0x2C) = src[2];
    int result = static_cast<int>(src[3]);
    CSF_AT(uint32_t, self, 0x30) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006FC1D0, CopyFourDwordsIntoLightAt24, "auto");
