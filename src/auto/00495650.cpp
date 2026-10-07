#include <cstdint>
#include "globals.h"
#include "hook.h"

// sub_495650: loads the pointer stored at [this+0x3E8] and stores the float
// constant 0.5f (0x3F000000) at [that+0x20]. The original tail-jumps into a
// shared code stub at 0x4A1CC0 which is not a function start, so the store is
// reproduced inline here.
static void __fastcall C_495650_SetChildField20ToHalf(void* self, void* /*edx*/)
{
    void* child = CSF_AT(void*, self, 0x3E8);
    CSF_AT(float, child, 0x20) = 0.5f;
}

HOOK(0x495650, C_495650_SetChildField20ToHalf, "auto");
