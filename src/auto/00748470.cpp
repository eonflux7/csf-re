#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the 64-bit spawn identifier stored at slot `index` (0..2) of the
// spawn-point block, or -1 for any other index. Pseudocode `this + 103/104/105`
// (QWORD units) is byte offsets 0x338/0x340/0x348.
static int64_t __fastcall MpSpawnPoints_GetSpawnIdentifier(void* self, void* /*edx*/, int index) {
    int64_t result = -1;
    switch (index) {
        case 0:
            return CSF_AT(int64_t, self, 0x338);
        case 1:
            return CSF_AT(int64_t, self, 0x340);
        case 2:
            return CSF_AT(int64_t, self, 0x348);
    }
    return result;
}

HOOK(0x748470, MpSpawnPoints_GetSpawnIdentifier, "auto");
