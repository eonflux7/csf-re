#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the entry at position `index` of the table stored in the
// object's field at +0x54 (each entry is one dword).
static int __fastcall C_00860bb8_GetEntryByIndex(C_00860bb8* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0054));
    return table[index];
}

HOOK(0x006CB8B0, C_00860bb8_GetEntryByIndex, "auto");
