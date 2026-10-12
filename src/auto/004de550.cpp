#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases every heap block owned by the element array at this->field_001c,
// then releases the array itself. The array header (element count) sits one
// dword before the element data; each element is 16 bytes and its first dword
// is an owned pointer.
static void __fastcall S_004de550_ReleaseElementBlocks(S_004de550* self, void* /*edx*/) {
    uint32_t arrayAddress = self->field_001c;
    if (arrayAddress == 0) {
        return;
    }

    uint8_t* arrayBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(arrayAddress));
    int32_t elementCount = *(reinterpret_cast<int32_t*>(arrayBase) - 1);

    // Walk the elements from last to first.
    for (int32_t elementIndex = elementCount - 1; elementIndex >= 0; --elementIndex) {
        uint32_t* elementFirstDword = reinterpret_cast<uint32_t*>(arrayBase + 16 * elementIndex);
        uint32_t ownedBlock = *elementFirstDword;
        if (ownedBlock != 0) {
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(ownedBlock)));
            *elementFirstDword = 0;
        }
    }

    // Free the array header block (count dword is at base - 4).
    Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(arrayAddress - 4)));
    self->field_001c = 0;
}

HOOK(0x4DE550, S_004de550_ReleaseElementBlocks, "auto");
