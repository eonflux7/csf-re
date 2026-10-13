#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Frees the block stored at +0x10 (if any) and clears the field.
// Returns the value of CoreMemory_Free when a block was freed, otherwise 0.
static int __fastcall SpatialQuery_Destroy(void* self, void* /*edx*/)
{
    int result = 0;
    void* block = CSF_AT(void*, self, 0x10);
    if (block)
    {
        result = CoreMemory_Free(block);
        CSF_AT(void*, self, 0x10) = nullptr;
    }
    return result;
}

HOOK(0x4013C0, SpatialQuery_Destroy, "auto");
