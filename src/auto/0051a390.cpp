#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Mem_FreeDebug: frees a block allocated by the game heap (debug variant).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the two owned buffers of a bdd_tables record (offsets 0x20 and 0x04,
// the latter only when its capacity at 0x18 is >= 0x10), zeroes the sizes and
// resets the capacity to 15 and the small-buffer flag at offset 4 to 0.
static int __stdcall BddTables_ResetRecordBuffers(int* a1) {
    // First owned buffer (offset 0x20): always freed when non-null.
    int result = CSF_AT(int, a1, 0x20);
    if (result) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result))));
    }
    CSF_AT(int, a1, 0x20) = 0;
    CSF_AT(int, a1, 0x24) = 0;
    CSF_AT(int, a1, 0x28) = 0;

    // Second buffer (offset 0x04): only when capacity (0x18, unsigned) >= 0x10.
    if (static_cast<uint32_t>(CSF_AT(int, a1, 0x18)) >= 0x10u) {
        result = CSF_AT(int, a1, 0x04);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result))));
        }
    }

    CSF_AT(int, a1, 0x14) = 0;
    CSF_AT(int, a1, 0x18) = 0xF;
    CSF_AT(uint8_t, a1, 0x04) = 0;
    return result;
}

HOOK(0x51A390, BddTables_ResetRecordBuffers, "auto");
