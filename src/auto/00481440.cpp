#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at index `index` of the table pointed to by field_0018.
static int __fastcall CCharWeaponFsm_GetIndexedEntry(CCharWeaponFsm* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(self->field_0018);
    return table[index];
}

HOOK(0x481440, CCharWeaponFsm_GetIndexedEntry, "auto");
