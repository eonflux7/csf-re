#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the block stored at +0x0C (if any) and clears the field.
// Returns the value of Mem_Free when a block was freed, otherwise 0.
static int __fastcall FreeAndClearField0C(void* self, void* /*edx*/)
{
    int result = 0;
    void* block = CSF_AT(void*, self, 0x0C);
    if (block)
    {
        result = Mem_Free(block);
        CSF_AT(void*, self, 0x0C) = nullptr;
    }
    return result;
}

HOOK(0x4E0990, FreeAndClearField0C, "auto");
