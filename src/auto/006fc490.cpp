#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at position `index` of the table whose address is stored
// in the object's field at +0x44 (each entry is one dword).
static int __fastcall C_00862e88_GetLightEntryAt(C_00862e88* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0044));
    return table[index];
}

HOOK(0x006FC490, C_00862e88_GetLightEntryAt, "auto");
