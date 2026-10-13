#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 3-dword block from *src into this->field_0030..0038 and clears field_002c.
// Returns the third dword copied (the original returns it in eax).
static int __fastcall C_00862598_CopyTriple(C_00862598* self, void* /*edx*/, uint32_t* src)
{
    self->field_002c = 0;
    uint32_t* dst = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x30);
    dst[0] = src[0];
    dst[1] = src[1];
    int result = static_cast<int>(src[2]);
    dst[2] = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006F26A0, C_00862598_CopyTriple, "auto");
