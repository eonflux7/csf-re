#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reference table of 32-bit values at 0x00861A90 (only the first entries are read here).
CSF_GLOBAL(int, g_Im3dReferenceDwordTable, 0x861A90);

// Compares `count` dwords starting at `address` against the reference table.
// Returns true when every compared element matches (or when count <= 0).
static bool __stdcall Im3d_DwordArrayMatchesReferenceTable(int address, int count) {
    const int* values = reinterpret_cast<const int*>(static_cast<uintptr_t>(address));
    int index = 0;
    if (count <= 0) {
        return true;
    }
    while (values[index] == (&g_Im3dReferenceDwordTable)[index]) {
        if (++index >= count) {
            return true;
        }
    }
    return false;
}

HOOK(0x6E3F10, Im3d_DwordArrayMatchesReferenceTable, "auto");
