#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the entry at position `index` of the table whose pointer is stored
// at the object's +0x6C field (each entry is one dword).
static int __fastcall C_0085ec18_GetEntryByIndex(C_0085ec18* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x6C)));
    return table[index];
}

HOOK(0x006BCA10, C_0085ec18_GetEntryByIndex, "auto");
