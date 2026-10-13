#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the heap block stored at +0x04 (if any) and clears the three
// 4-byte slots at +0x04, +0x08 and +0x0C of the owning structure.
static int __stdcall DestroyElementThroughContext(int* a1) {
    int result = CSF_AT(int, a1, 0x04);
    if (result) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(int, a1, 0x04) = 0;
    CSF_AT(int, a1, 0x08) = 0;
    CSF_AT(int, a1, 0x0C) = 0;
    return result;
}

HOOK(0x6B5220, DestroyElementThroughContext, "auto");
