#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_511260, 0x00511260, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the block held in slot +4 (via sub_511260 and Mem_FreeDebug), then zeroes slots +4, +8 and +0xC.
static int __fastcall BddLoader_ReleaseAndClearSlot(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x04);
    if (result) {
        sub_511260(result, CSF_AT(int, self, 0x08));
        result = CSF_AT(int, self, 0x04);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<intptr_t>(result)));
        }
    }
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x511220, BddLoader_ReleaseAndClearSlot, "auto");
