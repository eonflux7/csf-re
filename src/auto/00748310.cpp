#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns entry a3 from one of two per-object tables.
// a2 == 1 selects the table at +0x350, any other value selects the table at +0x358.
static int __fastcall LookupTableEntry(void* self, void* /*edx*/, char selector, int index) {
    if (selector == 1) {
        uint32_t* table = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x350));
        return static_cast<int>(table[index]);
    }
    uint32_t* table = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x358));
    return static_cast<int>(table[index]);
}

HOOK(0x00748310, LookupTableEntry, "auto");
