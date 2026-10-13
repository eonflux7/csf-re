#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at position `index` of the table whose address is stored
// in the object's field at +0x8C (each entry is one dword).
static int __fastcall GetParticleTableEntryAt(void* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8C)));
    return table[index];
}

HOOK(0x006F0020, GetParticleTableEntryAt, "auto");
