#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the dword at position `index` of the table whose pointer is stored
// in the object's field at +0x10 (each entry is one dword).
static int __fastcall ParticleEmitters_GetTableEntry(void* self, void* /*edx*/, int index) {
    int* table = CSF_AT(int*, self, 0x10);
    return table[index];
}

HOOK(0x006E0810, ParticleEmitters_GetTableEntry, "auto");
