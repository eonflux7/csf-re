#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees every entry of the slot table (each entry is a block whose first dword
// may hold a secondary block), clears the table slots, and resets the count.
static unsigned int __fastcall S_006b0320_FreeEntries(S_006b0320* self, void* /*edx*/)
{
    unsigned int result = self->field_0018;
    unsigned int index = 0;
    self->field_0008 = 0;

    if (result != 0) {
        do {
            uint32_t* entry = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint32_t*>(self->field_0010)[index]);
            if (entry != nullptr) {
                uint32_t innerBlock = entry[0];
                if (innerBlock != 0) {
                    Mem_Free(reinterpret_cast<void*>(innerBlock));
                    entry[0] = 0;
                }
                Mem_Free(entry);
                reinterpret_cast<uint32_t*>(self->field_0010)[index] = 0;
            }
            result = self->field_0018;
            ++index;
        } while (index < result);
        self->field_0018 = 0;
    } else {
        self->field_0018 = 0;
    }
    return result;
}

HOOK(0x006B0B10, S_006b0320_FreeEntries, "auto");
