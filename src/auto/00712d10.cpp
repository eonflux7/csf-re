#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Frees the block stored in the first field (if any), clears the field to 0,
// and returns the low byte of the free result (or 0 when nothing was stored).
static char __fastcall S_00694cd0_FreeFieldBlock(S_00694cd0* self, void* /*edx*/)
{
    uint32_t block = self->field_0000;
    char result = static_cast<char>(block);
    if (block)
    {
        result = static_cast<char>(CoreMemory_Free(reinterpret_cast<void*>(block)));
        self->field_0000 = 0;
    }
    return result;
}

HOOK(0x712D10, S_00694cd0_FreeFieldBlock, "auto");
