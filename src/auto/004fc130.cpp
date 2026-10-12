#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at position `index` of the table whose address is stored
// in the object's field at +0x04 (each entry is one dword).
static int __fastcall C_0085e7b8_GetSceneRegionAt(C_0085e7b8* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0004));
    return table[index];
}

HOOK(0x004FC130, C_0085e7b8_GetSceneRegionAt, "auto");
