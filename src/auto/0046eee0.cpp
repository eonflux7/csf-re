#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Constructs a two-dword record from a source pair. Writes the record's vtable,
// then copies the source's +4 and +8 dwords into the record's +4 and +8 slots.
// Returns the record pointer (null-safe: does nothing and returns null if obj is null).
static void* __stdcall MissionScript_ConstructPairFromSource(void* obj, void* sourcePair) {
    if (obj) {
        *(void***)obj = CSF_VTBL(0x008313F8);
        CSF_AT(uint32_t, obj, 4) = CSF_AT(uint32_t, sourcePair, 4);
        CSF_AT(uint32_t, obj, 8) = CSF_AT(uint32_t, sourcePair, 8);
    }
    return obj;
}

HOOK(0x0046EEE0, MissionScript_ConstructPairFromSource, "auto");
