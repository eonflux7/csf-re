#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the entry at position `index` of the table whose pointer is stored
// at the object's +0x1C field (each entry is one dword).
static int __fastcall C_00833708_GetEntryByIndex(C_00833708* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C)));
    return table[index];
}

HOOK(0x006BCA00, C_00833708_GetEntryByIndex, "auto");
