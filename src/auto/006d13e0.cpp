#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at position `index` of the table whose address is stored
// in the object's field at +0x64 (each entry is one dword).
static int __fastcall C_00860e68_GetLightHandleAt(C_00860e68* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0064));
    return table[index];
}

HOOK(0x006D13E0, C_00860e68_GetLightHandleAt, "auto");
